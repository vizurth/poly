#pragma once

#include <vector>
#include <string>

// Generate n-bit Gray code sequence as vectors of bits (0/1)
std::vector<std::vector<int>> generateGrayCodeBits(int n);

// Generate n-bit Gray code as strings, most significant bit first
std::vector<std::string> generateGrayCodeStrings(int n); 