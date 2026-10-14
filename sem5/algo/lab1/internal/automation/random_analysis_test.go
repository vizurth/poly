package automation

import (
	"fmt"
	"strings"
	"testing"
)

func TestRandomFieldMetrics(t *testing.T) {
	const (
		size  = 30
		steps = 200
		seed  = 20261012
	)

	rule, err := RuleFromVariant()
	if err != nil {
		t.Fatal(err)
	}

	field := New(size, rule)
	field.Randomize(seed)
	seen := make(map[string]int)
	cycleReported := false

	fmt.Println("METRICS,step,alive,dead")
	for step := 0; step <= steps; step++ {
		alive := 0
		for row := range field.cells {
			for _, cell := range field.cells[row] {
				if cell {
					alive++
				}
			}
		}
		if step == 0 && alive != 440 {
			t.Fatalf("unexpected initial live-cell count: got %d, want 440", alive)
		}
		fmt.Printf("METRICS,%d,%d,%d\n", step, alive, size*size-alive)

		state := field.stateKey()
		if firstStep, ok := seen[state]; ok && !cycleReported {
			fmt.Printf("CYCLE,start=%d,repeat=%d,period=%d\n", firstStep, step, step-firstStep)
			cycleReported = true
		} else if !ok {
			seen[state] = step
		}

		if step < steps {
			field.Step()
		}
	}
	if !cycleReported {
		t.Fatal("field did not repeat a state within the observation period")
	}
}

func (f *Field) stateKey() string {
	var state strings.Builder
	state.Grow(f.Size() * f.Size())
	for row := range f.cells {
		for _, cell := range f.cells[row] {
			if cell {
				state.WriteByte('1')
			} else {
				state.WriteByte('0')
			}
		}
	}
	return state.String()
}
