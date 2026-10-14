package gui

import "testing"

func TestCellSideForColumns(t *testing.T) {
	for _, test := range []struct {
		columns int
		want    float32
	}{
		{columns: 10, want: 32},
		{columns: 50, want: 16},
		{columns: 100, want: 8},
	} {
		if got := cellSideForColumns(test.columns); got != test.want {
			t.Errorf("cellSideForColumns(%d) = %v, want %v", test.columns, got, test.want)
		}
	}
}
