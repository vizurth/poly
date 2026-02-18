#include "lab1/include/shimbel.h"


/*
	LOOK: ShimbelSolver(const Graph<T>& g) : graph(g), n(g.getNumVertices())
	Конструктор класса
*/
template <typename T>
ShimbelSolver<T>::ShimbelSolver(const Graph<T>& g) : graph(g), n(g.getNumVertices()) {
    currentMatrix.resize(n, vector<T>(n, getINF()));
}

/*
	LOOK: void initializeMatrix()
	Иницилизируем матрицу Шимбела
*/
template <typename T>
void ShimbelSolver<T>::initializeMatrix() {
    // заполняем матрицу весами из графа
    // для несуществующих рёбер ставим INF
    T INF = getINF();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                currentMatrix[i][j] = 0; // нет петель в DAG
            } else if (graph.hasEdge(i, j)) {
                currentMatrix[i][j] = graph.getEdge(i, j);
            } else {
                currentMatrix[i][j] = INF;
            }
        }
    }
}

/*
	LOOK: void multiplyMatrix(bool findMin)
	Получаем флаг класса чтобы понимать что искать минимум или максимум
*/
template <typename T>
void ShimbelSolver<T>::multiplyMatrix(bool findMin) {
    // создаём новую матрицу для D^(k)
    T INF = getINF();
    vector<vector<T>> newMatrix(n, vector<T>(n, INF));

	// тут мы заменяем умножение матрицы на нахождение min/max пути от i до j вершины
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vector<T> possiblePaths;
            
            for (int p = 0; p < n; p++) {
                if (currentMatrix[i][p] != INF && currentMatrix[i][p] != 0 && graph.hasEdge(p, j)) {
                    T pathWeight = currentMatrix[i][p] + graph.getEdge(p, j);
                    possiblePaths.push_back(pathWeight);
                }
            }
            // проверка если пути или нет
            if (possiblePaths.empty()) {
                newMatrix[i][j] = INF;
            } else {
                if (findMin) {
                    newMatrix[i][j] = *min_element(possiblePaths.begin(), possiblePaths.end());
                } else {
                    newMatrix[i][j] = *max_element(possiblePaths.begin(), possiblePaths.end());
                }
            }
        }
    }
    
    currentMatrix = newMatrix;
}

/*
	LOOK: vector<vector<T>> computeShortestPaths(K) 
	Получаем матрицу шимбела минимальных путей для K вершин
*/
template <typename T>
vector<vector<T>> ShimbelSolver<T>::computeShortestPaths(int K) {
    if (K <= 0) {
        return vector<vector<T>>(n, vector<T>(n, getINF()));
    }
    
    initializeMatrix();
    
    // начитаем с k = 2 так как уже имеем матрицу смежности 
    for (int k = 2; k <= K; k++) {
        multiplyMatrix(true);
    }
    
    return currentMatrix;
}

/*
	LOOK: vector<vector<T>> computeShortestPaths(K) 
	Получаем матрицу шимбела минимальных путей для K вершин
*/
template <typename T>
vector<vector<T>> ShimbelSolver<T>::computeLongestPaths(int K) {
    if (K <= 0) {
        return vector<vector<T>>(n, vector<T>(n, getINF()));
    }
    
    initializeMatrix();
    
	// начитаем с k = 2 так как уже имеем матрицу смежности 
    for (int k = 2; k <= K; k++) {
        multiplyMatrix(false);
    }
    
    return currentMatrix;
}

/*
	LOOK: void printMatrix(matrix, title)
	Выводим матрицу Шимбела и пишем какой то title
*/
template <typename T>
void ShimbelSolver<T>::printMatrix(const vector<vector<T>>& matrix, const string& title) {
    int n = matrix.size();
    T INF = getINF();
    
    cout << "\n" << title << "\n";
    
    // шапка таблицы
    cout << "    ";
    for (int i = 0; i < n; i++) {
        cout << setw(8) << i;
    }
    cout << "\n";
    
    // строки матрицы
    for (int i = 0; i < n; i++) {
        cout << setw(3) << i << " ";
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != INF) {
                cout << setw(8) << fixed << setprecision(2) << matrix[i][j];
            } else {
                cout << setw(8) << "-";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}