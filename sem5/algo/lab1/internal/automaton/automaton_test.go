package automaton

import "testing"

func TestRuleFromVariantCalculatesRuleAndBits(t *testing.T) {
	rule, err := RuleFromVariant(1)
	if err != nil {
		t.Fatalf("RuleFromVariant returned error: %v", err)
	}
	if rule.Number != 3_442_296 {
		t.Fatalf("rule number = %d, want 3442296", rule.Number)
	}
	if rule.Bits() != "00000000001101001000011001111000" {
		t.Fatalf("rule bits = %q", rule.Bits())
	}
}

func TestRuleFromVariantRejectsNumberOutside32Bits(t *testing.T) {
	if _, err := RuleFromVariant(1_248); err == nil {
		t.Fatal("expected an error for a rule that does not fit into 32 bits")
	}
}

func TestStepUsesVariantRule(t *testing.T) {
	rule, _ := RuleFromVariant(1)
	field := New(3, rule)
	field.Set(0, 1, true)
	field.Set(2, 1, true)

	field.Step()

	if !field.At(1, 1) {
		t.Fatal("expected configuration 01010 to produce a live cell")
	}
}

func TestStepWrapsNeighboursAcrossFieldEdge(t *testing.T) {
	rule, _ := RuleFromVariant(1)
	field := New(3, rule)
	field.Set(0, 0, true)
	field.Set(0, 1, true)
	field.Set(0, 2, true)

	field.Step()

	if !field.At(0, 0) {
		t.Fatal("expected the left neighbour to wrap from the last column")
	}
}

func TestStepUpdatesAllCellsSimultaneously(t *testing.T) {
	rule, _ := RuleFromVariant(1)
	field := New(3, rule)
	field.Set(2, 1, true)
	field.Set(2, 2, true)

	field.Step()

	if field.LiveCount() != 0 {
		t.Fatal("expected every cell to be evaluated from the original field")
	}
}

func TestClearAndRandomize(t *testing.T) {
	rule, _ := RuleFromVariant(1)
	field := New(4, rule)
	field.Randomize(1)
	if field.LiveCount() == 0 {
		t.Fatal("expected deterministic randomization with seed 1 to create live cells")
	}

	field.Clear()
	if field.LiveCount() != 0 {
		t.Fatal("expected Clear to remove every live cell")
	}
}
