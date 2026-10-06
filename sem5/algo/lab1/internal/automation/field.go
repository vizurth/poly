package automation

import (
	"math/rand"
)

// Field — квадратное поле автомата.
type Field struct {
	cells [][]bool
	rule  Rule
}

func New(size int, rule Rule) *Field {
	return &Field{cells: makeCells(size), rule: rule}
}

func (f *Field) Size() int {
	return len(f.cells)
}

func (f *Field) At(row, col int) bool {
	return f.cells[f.wrap(row)][f.wrap(col)]
}

func (f *Field) Set(row, col int, state bool) {
	f.cells[f.wrap(row)][f.wrap(col)] = state
}

func (f *Field) Toggle(row, col int) {
	row, col = f.wrap(row), f.wrap(col)
	f.cells[row][col] = !f.cells[row][col]
}

func (f *Field) Clear() {
	for row := range f.cells {
		for col := range f.cells[row] {
			f.cells[row][col] = false
		}
	}
}

func (f *Field) Randomize(seed int64) {
	rng := rand.New(rand.NewSource(seed))
	for row := range f.cells {
		for col := range f.cells[row] {
			f.cells[row][col] = rng.Intn(2) == 1
		}
	}
}

// Step обновляем состояние поля исхотя из того как заполнены клетки на поле
func (f *Field) Step() {
	next := makeCells(f.Size())
	for row := range f.cells {
		for col := range f.cells[row] {
			// transition говорит получаем мы 1 или 0 из таблицы истинности
			next[row][col] = f.rule.transition(f.neighbourhoodIndex(row, col))
		}
	}
	f.cells = next
}

// neighbourhoodIndex возвращает позицию правила в таблице истинности (центр, верх, право, низ, лево) -> центр;
func (f *Field) neighbourhoodIndex(row, col int) int {
	bits := [5]bool{
		f.At(row, col),   // центр
		f.At(row-1, col), // верх
		f.At(row, col+1), // право
		f.At(row+1, col), // низ
		f.At(row, col-1), // лево
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

// wrap учитываем размер поля чтобы могли прыгать через границы
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
