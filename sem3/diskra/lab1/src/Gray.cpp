#include "../include/Gray.hpp"

static inline int grayOf(int x) { return x ^ (x >> 1); }

vector<string> generateGrayCode(int n){
	vector<string> gray;
	int total = 1 << n;
	for (int i = 0; i < total; ++i) {
		int g = grayOf(i);
		string code;
		for (int j = n - 1; j >= 0; --j) {
			code += ((g >> j) & 1) ? '1' : '0';
		}
		gray.push_back(code);
	}

	return gray;
}