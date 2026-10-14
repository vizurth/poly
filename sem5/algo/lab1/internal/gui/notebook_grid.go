package gui

import "fyne.io/fyne/v2"

const maxCellSide float32 = 32
const maxGridSide float32 = 800

func cellSideForColumns(columns int) float32 {
	if columns <= 0 {
		return maxCellSide
	}
	side := maxGridSide / float32(columns)
	if side > maxCellSide {
		return maxCellSide
	}
	return side
}

// notebookGridLayout раскладывает квадратные клетки как в тетрадной сетке.
type notebookGridLayout struct {
	columns   int
	cellSide  float32
	lineWidth float32
}

type topLeftGridLayout struct{}

func (layout topLeftGridLayout) Layout(objects []fyne.CanvasObject, _ fyne.Size) {
	for _, object := range objects {
		objectSize := object.MinSize()
		object.Move(fyne.NewPos(0, 0))
		object.Resize(objectSize)
	}
}

func (topLeftGridLayout) MinSize(objects []fyne.CanvasObject) fyne.Size {
	if len(objects) == 0 {
		return fyne.NewSize(0, 0)
	}
	return objects[0].MinSize()
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
