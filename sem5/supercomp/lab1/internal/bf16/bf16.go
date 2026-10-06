package bf16

import (
	"math"
)

const (
	exponentBits = 8
	mantissaBits = 7
	totalBits    = 1 + exponentBits + mantissaBits
	bias         = 127
	maxExponent  = (1 << exponentBits) - 1 // 11111111

	nanBits      uint16 = maxExponent<<mantissaBits | 1<<(mantissaBits-1) // 0 11111111 1000000
	infinityBits uint16 = maxExponent << mantissaBits                     // 0 11111111 0000000
)

// из десятичной формы в bf16
func FloatToBits(value float64) uint16 {
	var sign uint16
	if value < 0 {
		sign, value = 1, -value
	}
	if value != value { // NaN != NaN
		return nanBits
	}
	if value == 0 { // ноль со знаком
		return sign << 15
	}

	integer := uint64(value)
	integerBits := integerToBinary(integer)
	fractionBits := fractionToBinary(value - float64(integer))
	exponent, source, found := normalize(integerBits, fractionBits)
	// слишком маленькое число не смогли за 135 бит найти единицу следоватьльно оно уже не поместиться в bf16
	// 127 на поиск 1.xxxx и 7 + 1 на мантиссу и бит округления
	if !found {
		return sign << 15
	}
	mantissa, bump := roundMantissa(source)
	exponent += bump

	exponent += bias
	if exponent >= maxExponent { // -inf | +inf
		return sign<<15 | infinityBits
	}
	if exponent <= 0 { // слишком маленькое число если сдвинули меньше чем на 126 позиций 0.000000.....1
		return sign << 15
	}
	return sign<<15 | uint16(exponent<<7) | mantissa // S + E + M
}

// переводим целую часть в бинарный вид
func integerToBinary(value uint64) string {
	return toBinary(value, false)
}

// функция перевода числа в бинарный вид с флагом заполнения до 16 если нужно
func toBinary(value uint64, fillTo16 bool) string {
	bits := ""
	for {
		if value&1 == 1 {
			bits = "1" + bits
		} else {
			bits = "0" + bits
		}
		value >>= 1
		if value == 0 {
			break
		}
	}
	if fillTo16 {
		for len(bits) < totalBits {
			bits = "0" + bits
		}
	}
	return bits
}

// перевод дробной части числа в бинарный вид
func fractionToBinary(value float64) string {
	bits := ""
	for index := 0; value != 0 && index < 135; index++ {
		value *= 2
		if value >= 1 {
			bits += "1"
			value--
		} else {
			bits += "0"
		}
	}
	return bits
}

// функция помогает получить то на сколько сдвинули число до вида 1.xxxx и получить xxxx, и флаг того получилось ли нормализовать
func normalize(integerBits, fractionBits string) (int, string, bool) {
	if integerBits != "0" {
		return len(integerBits) - 1, integerBits[1:] + fractionBits, true
	}

	for index, bit := range fractionBits {
		if bit == '1' {
			return -(index + 1), fractionBits[index+1:], true
		}
	}
	return 0, "", false
}

// округляем мантиссу если последний бит равен 1 то округляем
func roundMantissa(source string) (uint16, int) {
	for len(source) < 8 {
		source += "0"
	}

	var mantissa uint16
	for _, bit := range source[:7] {
		mantissa <<= 1
		if bit == '1' {
			mantissa |= 1
		}
	}
	if source[7] == '1' { // если 8 бит равен 1 округляем мантиссу так как оброшенная часть >= половины
		mantissa++
	}
	if mantissa == 128 { // переполнение мантиссы ушло в экспоненту
		return 0, 1
	}
	return mantissa, 0
}

// переводим bf16 в десятичный вариант
func BitsToFloat(binary string) float64 {
	sign := 1.0
	if binary[0] == '1' {
		sign = -1
	}
	exponent := binaryToNumber(binary[1 : 1+exponentBits])
	mantissa := bitsToFraction(binary[1+exponentBits:])

	if exponent == maxExponent {
		zero := 0.0
		if mantissa != 0 {
			return zero / zero // NaN
		}
		return sign / zero // +Inf | -Inf
	}

	value, power := mantissa, 1-bias
	if exponent != 0 {
		value, power = 1+mantissa, int(exponent)-bias
	}
	return sign * value * math.Pow(2, float64(power))
}

// из бинарного вида в десятичный вид
func binaryToNumber(binary string) int {
	value := 0
	for _, bit := range binary {
		value <<= 1
		if bit == '1' {
			value++
		}
	}
	return value
}

// из бинарного вида в десятичную с точной
func bitsToFraction(binary string) float64 {
	fraction := 0.0
	for index, bit := range binary {
		if bit == '1' {
			fraction += math.Pow(2, -float64(index+1))
		}
	}
	return fraction
}

// из десятичного в бинарный с 16 битами
func BitsToBinary(bits uint16) string {
	return toBinary(uint64(bits), true)
}
