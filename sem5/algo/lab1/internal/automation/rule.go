package automation

import (
	"fmt"
	"math"
)

const ruleMultiplier uint32 = 11 * 2006 * 13 * 12

// Rule хранит таблицу переходов для клетки и четырёх соседей.
type Rule struct {
	Number uint32
}

// RuleFromVariant считает номер правила по номеру варианта.
func RuleFromVariant(variant int) (Rule, error) {
	if variant < 1 {
		return Rule{}, fmt.Errorf("variant number must be positive")
	}
	number := uint32(variant) * ruleMultiplier
	if number > math.MaxUint32 {
		return Rule{}, fmt.Errorf("variant number must not exceed %d", math.MaxUint32/ruleMultiplier)
	}
	return Rule{Number: number}, nil
}

// Bits возвращает правило в виде 32 бит.
func (r Rule) Bits() string {
	return fmt.Sprintf("%032b", r.Number)
}

func (r Rule) transition(configuration int) bool {
	// Нулевой вариант правила хранится в старшем бите.
	return r.Number&(1<<uint(31-configuration)) != 0
}
