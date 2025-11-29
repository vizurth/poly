string buildSDNF(const vector<string>& x, const vector<int>& F) {
    bool f = false; // признак наличия левого операнда дизъюнкции
    string result;
    int n = x.size();
    int total = 1 << n; // 2^n

    for (int i = 0; i < total; i++) {
        if (F[i] == 1) {
            if (f) {
                result += " ∨ "; // добавляем знак дизъюнкции
            } else {
                f = true; // первое слагаемое
            }

            bool g = false; // признак наличия левого операнда конъюнкции
            result += "(";

            for (int j = 0; j < n; j++) {
                if (g) {
                    result += " ∧ "; // добавляем знак конъюнкции
                } else {
                    g = true;
                }

                int v = (i >> (n - j - 1)) & 1; // значения j-го разряда числа i

                if (v == 0) {
                    result += "¬"; // инверсия
                }

                result += x[j]; // идентификатор переменной
            }

            result += ")";
        }
    }

    return result;
}

string buildSKNF(const vector<string>& x, const vector<int>& F) {
    bool f = false; // признак наличия левого операнда конъюнкции
    string result;
    int n = x.size();
    int total = 1 << n; // 2^n

    for (int i = 0; i < total; i++) {
		// СКНФ строится по нулям функции
        if (F[i] == 0) {
            if (f) {
                result += " ∧ "; // добавляем знак конъюнкции
            } else {
                f = true; // первая скобка
            }

            bool g = false; // признак левого операнда дизъюнкции
            result += "(";

            for (int j = 0; j < n; j++) {
                if (g) {
                    result += " ∨ "; // знак дизъюнкции
                } else {
                    g = true;
                }

                int v = (i >> (n - j - 1)) & 1; // j-й разряд числа i

                if (v == 1) {
                    result += "¬"; // здесь инверсия наоборот
                }

                result += x[j];
            }

            result += ")";
        }
    }

    return result;
}