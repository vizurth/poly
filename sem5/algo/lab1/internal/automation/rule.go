package automation

import (
	"fmt"
)

const variant = 2
const rule uint32 = 11 * 2006 * 13 * 12 * variant

// Rule хранит таблицу переходов для клетки и четырёх соседей.
type Rule struct {
	Number uint32
}

// RuleFromVariant считает номер правила по номеру варианта.
func RuleFromVariant() (Rule, error) {
	number := rule
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
