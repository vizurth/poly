package gui

import (
	"fmt"
	"testing"

	"fyne.io/fyne/v2/test"
	"github.com/vizurth/poly/sem5/algo/lab1/internal/automation"
)

func TestNewCreatesDefaultInteractiveField(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	rule, _ := automation.RuleFromVariant(1)
	ui := New(rule)

	if ui.field.Size() != defaultSize {
		t.Fatalf("field size = %d, want %d", ui.field.Size(), defaultSize)
	}
	if len(ui.grid.Objects) != defaultSize*defaultSize {
		t.Fatalf("grid has %d cells, want %d", len(ui.grid.Objects), defaultSize*defaultSize)
	}
}

func TestStepUpdatesField(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	rule, _ := automation.RuleFromVariant(1)
	ui := New(rule)
	ui.field.Set(0, 1, true)
	ui.field.Set(2, 1, true)
	ui.step()

	if !ui.field.At(1, 1) {
		t.Fatal("expected the center cell to become live after one manual step")
	}
}

func TestRunStepsAppliesRequestedNumberOfOperations(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	ui := New(automation.Rule{Number: 0xFFFF0000})
	ui.runSteps(2)

	if ui.field.At(0, 0) {
		t.Fatal("cell should be dead after two operations")
	}
	ui.runSteps(1)
	if !ui.field.At(0, 0) {
		t.Fatal("cell should be live after one more operation")
	}
}

func TestCreateFieldKeepsRulePassedFromCode(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	rule, _ := automation.RuleFromVariant(2)
	ui := New(rule)
	ui.size.SetText("5")
	ui.createField()

	want := fmt.Sprintf("Номер правила: %d\nБиты: %s", rule.Number, rule.Bits())
	if ui.rule.Text != want {
		t.Fatalf("rule text = %q, want %q", ui.rule.Text, want)
	}
}

func TestCreateFieldRejectsSizeAboveLimit(t *testing.T) {
	app := test.NewApp()
	defer app.Quit()

	rule, _ := automation.RuleFromVariant(1)
	ui := New(rule)
	ui.size.SetText("26")
	ui.createField()

	if ui.field.Size() != defaultSize {
		t.Fatalf("field size = %d, want unchanged size %d", ui.field.Size(), defaultSize)
	}
	if ui.message.Text == "" {
		t.Fatal("expected a validation message for size above 25")
	}
}
