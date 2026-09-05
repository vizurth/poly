// Package automaton implements the laboratory's two-dimensional cellular automaton.
package automaton

import (
	"fmt"
	"math"
	"math/rand"
)

const ruleMultiplier uint64 = 11 * 2006 * 13 * 12

// Rule is the 32-bit transition table for the five cells in a von Neumann neighbourhood.
type Rule struct {
	Number uint32
}

// RuleFromVariant calculates the rule specified by the laboratory assignment.
func RuleFromVariant(variant int) (Rule, error) {
	if variant < 1 {
		return Rule{}, fmt.Errorf("variant number must be positive")
	}
	number := uint64(variant) * ruleMultiplier
	if number > math.MaxUint32 {
		return Rule{}, fmt.Errorf("variant number must not exceed %d", math.MaxUint32/ruleMultiplier)
	}
	return Rule{Number: uint32(number)}, nil
}

// Bits returns the rule as a 32-bit binary transition table.
func (r Rule) Bits() string {
	return fmt.Sprintf("%032b", r.Number)
}

// Field is a square, binary cellular-automaton field.
type Field struct {
	cells [][]bool
	rule  Rule
}

// New creates a cleared field of size by size cells.
func New(size int, rule Rule) *Field {
	return &Field{cells: makeCells(size), rule: rule}
}

// Size returns one side of the square field.
func (f *Field) Size() int {
	return len(f.cells)
}

// At reports whether the cell is live. Coordinates wrap around field edges.
func (f *Field) At(row, col int) bool {
	return f.cells[f.wrap(row)][f.wrap(col)]
}

// Set changes one cell. Coordinates wrap around field edges.
func (f *Field) Set(row, col int, live bool) {
	f.cells[f.wrap(row)][f.wrap(col)] = live
}

// Toggle changes one cell from live to dead or back.
func (f *Field) Toggle(row, col int) {
	row, col = f.wrap(row), f.wrap(col)
	f.cells[row][col] = !f.cells[row][col]
}

// Clear makes every cell dead.
func (f *Field) Clear() {
	for row := range f.cells {
		for col := range f.cells[row] {
			f.cells[row][col] = false
		}
	}
}

// Randomize fills the field with independently selected live or dead cells.
func (f *Field) Randomize(seed int64) {
	rng := rand.New(rand.NewSource(seed))
	for row := range f.cells {
		for col := range f.cells[row] {
			f.cells[row][col] = rng.Intn(2) == 1
		}
	}
}

// LiveCount returns the number of live cells.
func (f *Field) LiveCount() int {
	count := 0
	for _, row := range f.cells {
		for _, live := range row {
			if live {
				count++
			}
		}
	}
	return count
}

// Step advances the complete field by one generation.
func (f *Field) Step() {
	next := makeCells(f.Size())
	for row := range f.cells {
		for col := range f.cells[row] {
			next[row][col] = f.rule.transition(f.neighbourhoodIndex(row, col))
		}
	}
	f.cells = next
}

func (r Rule) transition(configuration int) bool {
	return r.Number&(1<<uint(31-configuration)) != 0
}

func (f *Field) neighbourhoodIndex(row, col int) int {
	bits := [5]bool{
		f.At(row, col),
		f.At(row-1, col),
		f.At(row, col+1),
		f.At(row+1, col),
		f.At(row, col-1),
	}
	index := 0
	for _, bit := range bits {
		index <<= 1
		if bit {
			index++
		}
	}
	return index
}

func (f *Field) wrap(value int) int {
	size := f.Size()
	return (value%size + size) % size
}

func makeCells(size int) [][]bool {
	cells := make([][]bool, size)
	for row := range cells {
		cells[row] = make([]bool, size)
	}
	return cells
}
