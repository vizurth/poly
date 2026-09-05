package gui

import (
	"testing"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/test"
)

func TestNotebookGridPlacesSquareCellsWithoutGaps(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	grid := container.New(notebookGridLayout{columns: 2, cellSide: 24, lineWidth: 1},
		newCell(false, func() {}),
		newCell(false, func() {}),
		newCell(false, func() {}),
		newCell(false, func() {}),
	)
	grid.Resize(fyne.NewSize(48, 48))

	for index, object := range grid.Objects {
		if object.Size() != (fyne.NewSize(23, 23)) {
			t.Fatalf("cell %d size = %v, want 23×23", index, object.Size())
		}
	}
	if grid.Objects[1].Position() != (fyne.NewPos(24, 0)) {
		t.Fatalf("second cell position = %v, want (24, 0)", grid.Objects[1].Position())
	}
	if grid.Objects[2].Position() != (fyne.NewPos(0, 24)) {
		t.Fatalf("third cell position = %v, want (0, 24)", grid.Objects[2].Position())
	}
}
