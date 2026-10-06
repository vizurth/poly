package gui

import "fyne.io/fyne/v2"

// notebookGridLayout раскладывает квадратные клетки как в тетрадной сетке.
type notebookGridLayout struct {
	columns   int
	cellSide  float32
	lineWidth float32
}

type centeredGridLayout struct {
	size float32
}

func (layout centeredGridLayout) Layout(objects []fyne.CanvasObject, size fyne.Size) {
	for _, object := range objects {
		objectSize := object.MinSize()
		object.Move(fyne.NewPos((size.Width-objectSize.Width)/2, (size.Height-objectSize.Height)/2))
		object.Resize(objectSize)
	}
}

func (layout centeredGridLayout) MinSize([]fyne.CanvasObject) fyne.Size {
	return fyne.NewSize(layout.size, layout.size)
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
