package gui

import (
	"fmt"
	"testing"

	"fyne.io/fyne/v2/test"
	"github.com/vizurth/poly/sem5/algo/lab1/internal/automaton"
)

func TestNewCreatesDefaultInteractiveField(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	rule, _ := automaton.RuleFromVariant(1)
	ui := New(rule)

	if ui.field.Size() != defaultSize {
		t.Fatalf("field size = %d, want %d", ui.field.Size(), defaultSize)
	}
	if len(ui.grid.Objects) != defaultSize*defaultSize {
		t.Fatalf("grid has %d cells, want %d", len(ui.grid.Objects), defaultSize*defaultSize)
	}
	if ui.status.Text != "Живых клеток: 0 · поколение: 0" {
		t.Fatalf("unexpected status: %q", ui.status.Text)
	}
}

func TestStepUpdatesGenerationAndStatus(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	rule, _ := automaton.RuleFromVariant(1)
	ui := New(rule)
	ui.step()

	if ui.generation != 1 {
		t.Fatalf("generation = %d, want 1", ui.generation)
	}
	if ui.status.Text != "Живых клеток: 0 · поколение: 1" {
		t.Fatalf("unexpected status: %q", ui.status.Text)
	}
}

func TestCreateFieldKeepsRulePassedFromCode(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	rule, _ := automaton.RuleFromVariant(2)
	ui := New(rule)
	ui.size.SetText("5")
	ui.createField()

	want := fmt.Sprintf("Номер правила: %d\nБиты: %s", rule.Number, rule.Bits())
	if ui.rule.Text != want {
		t.Fatalf("rule text = %q, want %q", ui.rule.Text, want)
	}
}
