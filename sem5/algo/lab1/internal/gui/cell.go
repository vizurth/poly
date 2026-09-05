package gui

import (
	"image/color"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/widget"
)

var (
	liveCellColor = color.NRGBA{A: 255}
	deadCellColor = color.NRGBA{R: 255, G: 255, B: 255, A: 255}
	gridLineColor = color.NRGBA{R: 160, G: 160, B: 160, A: 255}
)

// cell is a full-size, clickable square in the field grid.
type cell struct {
	widget.BaseWidget
	live  bool
	onTap func()
}

func newCell(live bool, onTap func()) *cell {
	cell := &cell{live: live, onTap: onTap}
	cell.ExtendBaseWidget(cell)
	return cell
}

func (cell *cell) CreateRenderer() fyne.WidgetRenderer {
	fillColor := deadCellColor
	if cell.live {
		fillColor = liveCellColor
	}
	rectangle := canvas.NewRectangle(fillColor)
	return widget.NewSimpleRenderer(rectangle)
}

func (cell *cell) MinSize() fyne.Size {
	return fyne.NewSize(16, 16)
}

func (cell *cell) Tapped(_ *fyne.PointEvent) {
	cell.onTap()
}
