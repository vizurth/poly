#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>

// Multiset over a fixed universe of size N; stores multiplicities per element index [0..N-1]
class Multiset {
private:
	std::vector<int> multiplicities; // multiplicity per universe element index

public:
	Multiset() = default;
	explicit Multiset(std::size_t universeSize, int initialMultiplicity = 0);
	explicit Multiset(const std::vector<int>& values);

	std::size_t size() const noexcept;
	int& operator[](std::size_t index);
	int operator[](std::size_t index) const;

	const std::vector<int>& data() const noexcept { return multiplicities; }
	std::vector<int>& data() noexcept { return multiplicities; }

	// Set-like operations (on multiplicities)
	// Union: max per coordinate
	friend Multiset operator|(const Multiset& a, const Multiset& b);
	// Intersection: min per coordinate
	friend Multiset operator&(const Multiset& a, const Multiset& b);
	// Difference A \ B: max(0, a - b)
	friend Multiset operator-(const Multiset& a, const Multiset& b);
	// Symmetric difference: |a - b|
	friend Multiset operator^(const Multiset& a, const Multiset& b);

	// Complement with respect to capacity vector C (same size) provided separately
	// Returns C - A, clamped at >= 0.
	Multiset complement(const Multiset& capacity) const;
	// Relative complement: B \\ A = complement of A w.r.t B's multiplicities
	friend Multiset relative_complement(const Multiset& a, const Multiset& b);

	// Arithmetic operations on multiplicities (component-wise)
	friend Multiset add(const Multiset& a, const Multiset& b);
	friend Multiset sub(const Multiset& a, const Multiset& b);
	friend Multiset mul(const Multiset& a, const Multiset& b);
	// Division: safe integer division per coordinate (b[i] != 0 ? a[i]/b[i] : 0)
	friend Multiset div_euclid(const Multiset& a, const Multiset& b);

	// Pretty print
	std::string toString() const;
	void print(std::ostream& os = std::cout) const;

	// Validation helper
	void ensureSameSize(const Multiset& other, const char* opName) const;
}; 