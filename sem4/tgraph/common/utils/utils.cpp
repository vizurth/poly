#include "utils.h"
#include <iomanip>
#include <iostream>

/*
    LOOK: void printMatrix(const vector<vector<T>>& matrix, const string& title)
    Универсальная функция для печати матрицы с заголовком
*/
template <typename T>
void printMatrix(const vector<vector<T>> &matrix, const string &title) {
	const T POS_INF = numeric_limits<T>::max() / 2;
	const T NEG_INF = -numeric_limits<T>::max() / 2;
	int n = matrix.size();

	cout << "\n" << title << "\n";

	// заголовок (номера столбцов)
	cout << "    ";
	for (int i = 0; i < n; i++) {
		cout << setw(8) << i;
	}
	cout << "\n";

	// строки матрицы
	for (int i = 0; i < n; i++) {
		cout << setw(3) << i << " ";
		for (int j = 0; j < n; j++) {
			T val = matrix[i][j];
			if (val >= POS_INF || val <= NEG_INF) {
				cout << setw(8) << "-"; // недостижимо
			} else if (val == T{}) {
				cout << setw(8) << "-"; // нет ребра
			} else {
				cout << setw(8) << fixed << setprecision(2) << val;
			}
		}
		cout << "\n";
	}
}