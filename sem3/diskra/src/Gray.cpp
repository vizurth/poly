#include "../include/Gray.hpp"

#include <stdexcept>

static inline int grayOf(int x) { return x ^ (x >> 1); }

std::vector<std::vector<int>> generateGrayCodeBits(int n) {
	if (n <= 0 || n > 20) {
		throw std::invalid_argument("n must be in 1..20");
	}
	const int total = 1 << n;
	std::vector<std::vector<int>> seq;
	seq.reserve(total);
	for (int i = 0; i < total; ++i) {
		int g = grayOf(i);
		std::vector<int> bits(n, 0);
		for (int b = 0; b < n; ++b) {
			int mask = 1 << (n - 1 - b);
			bits[b] = (g & mask) ? 1 : 0;
		}
		seq.push_back(bits);
	}
	return seq;
}

std::vector<std::string> generateGrayCodeStrings(int n) {
	auto bits = generateGrayCodeBits(n);
	std::vector<std::string> res;
	res.reserve(bits.size());
	for (const auto& row : bits) {
		std::string s;
		s.reserve(row.size());
		for (int v : row) s.push_back(v ? '1' : '0');
		res.push_back(std::move(s));
	}
	return res;
} 