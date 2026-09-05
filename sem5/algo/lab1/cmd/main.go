package main

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"github.com/vizurth/poly/sem5/algo/lab1/internal/automaton"
	"github.com/vizurth/poly/sem5/algo/lab1/internal/gui"
)

// variantNumber is the laboratory variant number. Change it here when needed.
const variantNumber = 1

func main() {
	rule, err := automaton.RuleFromVariant(variantNumber)
	if err != nil {
		panic(err)
	}

	application := app.New()
	window := application.NewWindow("Клеточный автомат — лабораторная 1")
	window.SetContent(gui.New(rule).Content())
	window.Resize(fyne.NewSize(760, 760))
	window.ShowAndRun()
}
