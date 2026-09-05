package gui

import (
	"image/color"
	"testing"

	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/test"
)

func TestLiveCellIsFullyFilledWithLiveColor(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	cell := newCell(true, func() {})
	renderer := cell.CreateRenderer()
	rectangle, ok := renderer.Objects()[0].(*canvas.Rectangle)
	if !ok {
		t.Fatal("cell renderer must draw a filled rectangle")
	}
	if rectangle.FillColor != (color.NRGBA{A: 255}) {
		t.Fatalf("live cell color = %v, want black", rectangle.FillColor)
	}
}

func TestDeadCellIsWhite(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	cell := newCell(false, func() {})
	rectangle := cell.CreateRenderer().Objects()[0].(*canvas.Rectangle)
	if rectangle.FillColor != (color.NRGBA{R: 255, G: 255, B: 255, A: 255}) {
		t.Fatalf("dead cell color = %v, want white", rectangle.FillColor)
	}
}

func TestCellCallsClickHandler(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	clicked := false
	cell := newCell(false, func() { clicked = true })
	cell.Tapped(nil)

	if !clicked {
		t.Fatal("expected a click on a cell to call its handler")
	}
}
