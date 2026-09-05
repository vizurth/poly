package gui

import "fyne.io/fyne/v2"

// notebookGridLayout puts same-size square cells edge to edge, like graph paper.
type notebookGridLayout struct {
	columns   int
	cellSide  float32
	lineWidth float32
}

func (layout notebookGridLayout) Layout(objects []fyne.CanvasObject, _ fyne.Size) {
	for index, object := range objects {
		column := index % layout.columns
		row := index / layout.columns
		object.Move(fyne.NewPos(float32(column)*layout.cellSide, float32(row)*layout.cellSide))
		visibleSide := layout.cellSide - layout.lineWidth
		object.Resize(fyne.NewSize(visibleSide, visibleSide))
	}
}

func (layout notebookGridLayout) MinSize(objects []fyne.CanvasObject) fyne.Size {
	rows := (len(objects) + layout.columns - 1) / layout.columns
	return fyne.NewSize(float32(layout.columns)*layout.cellSide, float32(rows)*layout.cellSide)
}
