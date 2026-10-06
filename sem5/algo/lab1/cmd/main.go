package main

import (
	"fmt"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"github.com/vizurth/poly/sem5/algo/lab1/internal/automation"
	"github.com/vizurth/poly/sem5/algo/lab1/internal/gui"
)

// Номер варианта задаётся здесь.
const variantNumber = 2

func main() {
	rule, err := automation.RuleFromVariant(variantNumber)
	fmt.Println(rule.Bits())
	if err != nil {
		panic(err)
	}

	application := app.New()
	window := application.NewWindow("Клеточный автомат — лабораторная 1")
	window.SetContent(gui.New(rule).Content())
	window.Resize(fyne.NewSize(700, 850))
	window.ShowAndRun()
}
