// Package gui собирает интерфейс приложения.
package gui

import (
	"fmt"
	"strconv"
	"time"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
	"github.com/vizurth/poly/sem5/algo/lab1/internal/automation"
)

const defaultSize = 10
const maxSize = 20

const cellSide float32 = 28
const gridLineWidth float32 = 1

// UI связывает поле автомата с кнопками и настройками.
type UI struct {
	field         *automation.Field
	automatonRule automation.Rule
	size          *widget.Entry
	iterations    *widget.Entry
	rule          *widget.Label
	message       *widget.Label
	grid          *fyne.Container
	board         *fyne.Container
	runID         int
}

// New создаёт интерфейс для указанного правила.
func New(rule automation.Rule) *UI {
	ui := &UI{
		field:         automation.New(defaultSize, rule),
		automatonRule: rule,
		size:          widget.NewEntry(),
		iterations:    widget.NewEntry(),
		rule:          widget.NewLabel(""),
		message:       widget.NewLabel(""),
		grid:          container.New(notebookGridLayout{columns: defaultSize, cellSide: cellSide, lineWidth: gridLineWidth}),
	}
	background := canvas.NewRectangle(gridLineColor)
	centeredGrid := container.New(centeredGridLayout{size: maxSize * cellSide}, ui.grid)
	ui.board = container.NewMax(background, centeredGrid)
	ui.size.SetText(strconv.Itoa(defaultSize))
	ui.iterations.SetText("30")
	ui.updateRule(rule)
	ui.refresh()
	return ui
}

// Content возвращает содержимое окна.
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
		ui.refresh()
	})
	step := widget.NewButton("Один шаг", func() {
		ui.cancelRun()
		ui.step()
	})
	run := widget.NewButton("Запустить", ui.run)

	settings := container.NewGridWithColumns(2,
		widget.NewLabel("Размер поля (5–20):"), ui.size,
		widget.NewLabel("Количество операций (1–1000):"), ui.iterations,
	)
	buttons := container.NewGridWithColumns(3, create, randomize, clear, step, run)
	header := container.NewVBox(title, ui.rule, settings, buttons, ui.message)
	return container.NewBorder(header, nil, nil, nil, container.NewScroll(ui.board))
}

func (ui *UI) createField() {
	size, err := positiveNumber(ui.size, 5, maxSize)
	if err != nil {
		ui.message.SetText(err.Error())
		return
	}
	ui.cancelRun()
	ui.field = automation.New(size, ui.automatonRule)
	ui.message.SetText("")
	ui.refresh()
}

func (ui *UI) run() {
	steps, err := positiveNumber(ui.iterations, 1, 1_000)
	if err != nil {
		ui.message.SetText(err.Error())
		return
	}
	ui.message.SetText("")
	ui.cancelRun()
	currentRun := ui.runID
	go func() {
		for range steps {
			time.Sleep(120 * time.Millisecond)
			fyne.Do(func() {
				if ui.runID == currentRun {
					ui.runSteps(1)
				}
			})
		}
	}()
}

func (ui *UI) cancelRun() {
	ui.runID++
}

func (ui *UI) runSteps(steps int) {
	for range steps {
		ui.field.Step()
	}
	ui.refresh()
}

func (ui *UI) step() {
	ui.field.Step()
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
}

func (ui *UI) updateRule(rule automation.Rule) {
	ui.rule.SetText(fmt.Sprintf("Номер правила: %d\nБиты: %s", rule.Number, rule.Bits()))
}

func positiveNumber(entry *widget.Entry, min, max int) (int, error) {
	value, err := strconv.Atoi(entry.Text)
	if err != nil || value < min || value > max {
		return 0, fmt.Errorf("введите целое число от %d до %d", min, max)
	}
	return value, nil
}
