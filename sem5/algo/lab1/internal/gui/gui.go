// Package gui contains the application's Fyne user interface.
package gui

import (
	"fmt"
	"strconv"
	"time"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
	"github.com/vizurth/poly/sem5/algo/lab1/internal/automaton"
)

const defaultSize = 20

const cellSide float32 = 28
const gridLineWidth float32 = 1

// UI connects form controls with a cellular-automaton field.
type UI struct {
	field         *automaton.Field
	automatonRule automaton.Rule
	size          *widget.Entry
	iterations    *widget.Entry
	rule          *widget.Label
	status        *widget.Label
	grid          *fyne.Container
	board         *fyne.Container
	generation    int
	runID         int
}

// New creates a ready-to-display automaton interface.
func New(rule automaton.Rule) *UI {
	ui := &UI{
		field:         automaton.New(defaultSize, rule),
		automatonRule: rule,
		size:          widget.NewEntry(),
		iterations:    widget.NewEntry(),
		rule:          widget.NewLabel(""),
		status:        widget.NewLabel(""),
		grid:          container.New(notebookGridLayout{columns: defaultSize, cellSide: cellSide, lineWidth: gridLineWidth}),
	}
	ui.board = container.NewMax(canvas.NewRectangle(gridLineColor), ui.grid)
	ui.size.SetText(strconv.Itoa(defaultSize))
	ui.iterations.SetText("30")
	ui.updateRule(rule)
	ui.refresh()
	return ui
}

// Content returns the root widget for a Fyne window.
func (ui *UI) Content() fyne.CanvasObject {
	title := widget.NewLabelWithStyle(
		"Двумерный клеточный автомат",
		fyne.TextAlignCenter,
		fyne.TextStyle{Bold: true},
	)
	ui.rule.Wrapping = fyne.TextWrapWord

	create := widget.NewButton("Создать поле", ui.createField)
	randomize := widget.NewButton("Случайно", func() {
		ui.cancelRun()
		ui.field.Randomize(time.Now().UnixNano())
		ui.refresh()
	})
	clear := widget.NewButton("Очистить", func() {
		ui.cancelRun()
		ui.field.Clear()
		ui.generation = 0
		ui.refresh()
	})
	step := widget.NewButton("Один шаг", func() {
		ui.cancelRun()
		ui.step()
	})
	run := widget.NewButton("Запустить", ui.run)
	stop := widget.NewButton("Стоп", ui.cancelRun)

	settings := container.NewGridWithColumns(2,
		widget.NewLabel("Размер поля (5–40):"), ui.size,
		widget.NewLabel("Количество шагов:"), ui.iterations,
	)
	buttons := container.NewGridWithColumns(3, create, randomize, clear, step, run, stop)
	header := container.NewVBox(title, ui.rule, settings, buttons, ui.status)
	return container.NewBorder(header, nil, nil, nil, container.NewScroll(ui.board))
}

func (ui *UI) createField() {
	size, err := positiveNumber(ui.size, 5, 40)
	if err != nil {
		ui.status.SetText(err.Error())
		return
	}
	ui.cancelRun()
	ui.field = automaton.New(size, ui.automatonRule)
	ui.generation = 0
	ui.refresh()
}

func (ui *UI) run() {
	steps, err := positiveNumber(ui.iterations, 1, 1_000)
	if err != nil {
		ui.status.SetText(err.Error())
		return
	}
	ui.runID++
	currentRun := ui.runID
	go func() {
		for range steps {
			time.Sleep(120 * time.Millisecond)
			fyne.Do(func() {
				if ui.runID == currentRun {
					ui.step()
				}
			})
		}
	}()
}

func (ui *UI) cancelRun() {
	ui.runID++
	ui.updateStatus()
}

func (ui *UI) step() {
	ui.field.Step()
	ui.generation++
	ui.refresh()
}

func (ui *UI) refresh() {
	objects := make([]fyne.CanvasObject, 0, ui.field.Size()*ui.field.Size())
	for row := 0; row < ui.field.Size(); row++ {
		for col := 0; col < ui.field.Size(); col++ {
			clickedRow, clickedCol := row, col
			cell := newCell(ui.field.At(row, col), func() {
				ui.cancelRun()
				ui.field.Toggle(clickedRow, clickedCol)
				ui.refresh()
			})
			objects = append(objects, cell)
		}
	}
	ui.grid.Layout = notebookGridLayout{columns: ui.field.Size(), cellSide: cellSide, lineWidth: gridLineWidth}
	ui.grid.Objects = objects
	ui.grid.Refresh()
	ui.updateStatus()
}

func (ui *UI) updateStatus() {
	ui.status.SetText(fmt.Sprintf("Живых клеток: %d · поколение: %d", ui.field.LiveCount(), ui.generation))
}

func (ui *UI) updateRule(rule automaton.Rule) {
	ui.rule.SetText(fmt.Sprintf("Номер правила: %d\nБиты: %s", rule.Number, rule.Bits()))
}

func positiveNumber(entry *widget.Entry, min, max int) (int, error) {
	value, err := strconv.Atoi(entry.Text)
	if err != nil || value < min || value > max {
		return 0, fmt.Errorf("введите целое число от %d до %d", min, max)
	}
	return value, nil
}
