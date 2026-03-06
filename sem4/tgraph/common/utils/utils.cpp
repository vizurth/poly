#include <iomanip>
#include <iostream>

/*
    LOOK: void printMatrix(const vector<vector<T>>& matrix, const string& title)
    Универсальная функция для печати матрицы с заголовком
*/
template <typename T>
void printMatrix(const vector<vector<T>> &matrix, const string &title) {
	int numColumns = matrix.size();
	cout << "\n" << title << "\n";

	// вывести заголовок (номера столбцов)
	cout << "    ";
	for (int i = 0; i < numColumns; i++) {
		cout << setw(8) << i;
	}
	cout << "\n";

	// вывести строки матрицы
	for (int i = 0; i < numColumns; i++) {
		cout << setw(3) << i << " ";
		for (int j = 0; j < numColumns; j++) {
			if (matrix[i][j] != 0) {
				cout << setw(8) << fixed << setprecision(2) << matrix[i][j];
			} else {
				cout << setw(8) << "-";
			}
		}
		cout << "\n";
	}
}
