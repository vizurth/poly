#include "utils.h"
#include <iomanip>
#include <iostream>

/*
    LOOK: void printMatrix(const vector<vector<T>>& matrix, const string& title)
    Универсальная функция для печати матрицы с заголовком
*/
template <typename T>
void printMatrix(const vector<vector<T>> &matrix, const string &title) {
	const T POS_INF = INF<T> / 2;
	const T NEG_INF = -INF<T> / 2;
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
			if (i == j) {
				cout << setw(8) << "0";
			} else if (val >= POS_INF || val <= NEG_INF || val == T{} ||
			           val == INF<T>) {
				cout << setw(8) << "-";
			} else {
				cout << setw(8) << fixed << setprecision(2) << val;
			}
		}
		cout << "\n";
	}
}

/*
    LOOK: void printVector(const vector<T>& vec, const string& title)
    Универсальная функция для печати вектора с заголовком
*/
template <typename T>
void printVector(const vector<T> &vec, const string &title) {
	cout << "\n" << title << "\n";
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i] == INF<T>) {
			cout << "  " << i << ": -\n"; // недостижимо
		} else {
			cout << "  " << i << ": " << vec[i] << "\n";
		}
	}
}