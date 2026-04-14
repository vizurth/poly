package seeder

import (
	"fmt"
	"strings"
)

func genRegNumber(i int) string {
	chars := []rune("АВЕКМНОРСТУХ")
	n := i % 999
	c1 := chars[(i/999)%len(chars)]
	c2 := chars[(i/(999*len(chars)))%len(chars)]
	c3 := chars[(i/(999*len(chars)*len(chars)))%len(chars)]
	region := 77 + (i % 5)
	return fmt.Sprintf("%c%03d%c%c%d", c1, n+1, c2, c3, region)
}

func transliterate(s string) string {
	mapping := map[rune]string{
		'А': "A", 'Б': "B", 'В': "V", 'Г': "G", 'Д': "D", 'Е': "E", 'Ё': "E", 'Ж': "ZH",
		'З': "Z", 'И': "I", 'Й': "Y", 'К': "K", 'Л': "L", 'М': "M", 'Н': "N", 'О': "O",
		'П': "P", 'Р': "R", 'С': "S", 'Т': "T", 'У': "U", 'Ф': "F", 'Х': "H", 'Ц': "TS",
		'Ч': "CH", 'Ш': "SH", 'Щ': "SCH", 'Ъ': "", 'Ы': "Y", 'Ь': "", 'Э': "E", 'Ю': "YU", 'Я': "YA",
		'а': "a", 'б': "b", 'в': "v", 'г': "g", 'д': "d", 'е': "e", 'ё': "e", 'ж': "zh",
		'з': "z", 'и': "i", 'й': "y", 'к': "k", 'л': "l", 'м': "m", 'н': "n", 'о': "o",
		'п': "p", 'р': "r", 'с': "s", 'т': "t", 'у': "u", 'ф': "f", 'х': "h", 'ц': "ts",
		'ч': "ch", 'ш': "sh", 'щ': "sch", 'ъ': "", 'ы': "y", 'ь': "", 'э': "e", 'ю': "yu", 'я': "ya",
	}
	var sb strings.Builder
	for _, r := range s {
		if val, ok := mapping[r]; ok {
			sb.WriteString(val)
		} else {
			sb.WriteRune(r)
		}
	}
	return sb.String()
}
