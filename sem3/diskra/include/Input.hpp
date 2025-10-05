#pragma once

#include <string>
#include <vector>
#include <optional>

int readInt(const std::string& prompt, int minValue, int maxValue);

// Read a vector<int> of given size with non-negative entries (for multiplicities)
std::vector<int> readVector(const std::string& prompt, std::size_t size, int minValue, int maxValue);

// Read choice from 1..k
int readChoice(const std::string& prompt, int k); 