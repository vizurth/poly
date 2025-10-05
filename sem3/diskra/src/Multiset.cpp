#include "../include/Multiset.hpp"

#include <sstream>

Multiset::Multiset(std::size_t universeSize, int initialMultiplicity)
	: multiplicities(universeSize, initialMultiplicity) {
	if (initialMultiplicity < 0) {
		throw std::invalid_argument("Initial multiplicity must be non-negative");
	}
}

Multiset::Multiset(const std::vector<int>& values)
	: multiplicities(values) {
	for (int v : multiplicities) {
		if (v < 0) throw std::invalid_argument("Multiplicity must be non-negative");
	}
}

std::size_t Multiset::size() const noexcept { return multiplicities.size(); }

int& Multiset::operator[](std::size_t index) {
	return multiplicities.at(index);
}

int Multiset::operator[](std::size_t index) const {
	return multiplicities.at(index);
}

void Multiset::ensureSameSize(const Multiset& other, const char* opName) const {
	if (size() != other.size()) {
		throw std::invalid_argument(std::string(opName) + ": multisets must be of same size");
	}
}

Multiset operator|(const Multiset& a, const Multiset& b) {
	a.ensureSameSize(b, "union");
	Multiset res(a.size());
	for (std::size_t i = 0; i < a.size(); ++i) {
		res[i] = std::max(a[i], b[i]);
	}
	return res;
}

Multiset operator&(const Multiset& a, const Multiset& b) {
	a.ensureSameSize(b, "intersection");
	Multiset res(a.size());
	for (std::size_t i = 0; i < a.size(); ++i) {
		res[i] = std::min(a[i], b[i]);
	}
	return res;
}

Multiset operator-(const Multiset& a, const Multiset& b) {
	a.ensureSameSize(b, "difference");
	Multiset res(a.size());
	for (std::size_t i = 0; i < a.size(); ++i) {
		int v = a[i] - b[i];
		res[i] = v > 0 ? v : 0;
	}
	return res;
}

Multiset operator^(const Multiset& a, const Multiset& b) {
	a.ensureSameSize(b, "symmetric difference");
	Multiset res(a.size());
	for (std::size_t i = 0; i < a.size(); ++i) {
		res[i] = std::abs(a[i] - b[i]);
	}
	return res;
}

Multiset Multiset::complement(const Multiset& capacity) const {
	ensureSameSize(capacity, "complement");
	Multiset res(size());
	for (std::size_t i = 0; i < size(); ++i) {
		int v = capacity[i] - (*this)[i];
		res[i] = v > 0 ? v : 0;
	}
	return res;
}

Multiset relative_complement(const Multiset& a, const Multiset& b) {
	// b \\ a
	return b - a;
}

Multiset add(const Multiset& a, const Multiset& b) {
	a.ensureSameSize(b, "add");
	Multiset res(a.size());
	for (std::size_t i = 0; i < a.size(); ++i) res[i] = a[i] + b[i];
	return res;
}

Multiset sub(const Multiset& a, const Multiset& b) {
	a.ensureSameSize(b, "sub");
	Multiset res(a.size());
	for (std::size_t i = 0; i < a.size(); ++i) res[i] = a[i] - b[i];
	return res;
}

Multiset mul(const Multiset& a, const Multiset& b) {
	a.ensureSameSize(b, "mul");
	Multiset res(a.size());
	for (std::size_t i = 0; i < a.size(); ++i) res[i] = a[i] * b[i];
	return res;
}

Multiset div_euclid(const Multiset& a, const Multiset& b) {
	a.ensureSameSize(b, "div");
	Multiset res(a.size());
	for (std::size_t i = 0; i < a.size(); ++i) {
		res[i] = (b[i] != 0) ? (a[i] / b[i]) : 0;
	}
	return res;
}

std::string Multiset::toString() const {
	std::ostringstream oss;
	oss << "{";
	for (std::size_t i = 0; i < size(); ++i) {
		oss << multiplicities[i];
		if (i + 1 < size()) oss << ", ";
	}
	oss << "}";
	return oss.str();
}

void Multiset::print(std::ostream& os) const {
	os << toString();
} 