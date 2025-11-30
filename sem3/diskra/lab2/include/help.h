#pragma once

string buildSDNF(const vector<string>& x, const vector<int>& F) {
    bool f = false; // первый операнд дизъюнкции
    string result;
    int n = x.size();
    int total = 1 << n; // 2^n

    for (int i = 0; i < total; i++) {
		// СДНФ строится по единицам функции
        if (F[i] == 1) {
            if (f) {
                result += " ∨ ";
            } else {
                f = true; // первое выражение
            }
            bool g = false; // первый операнд конъюнкции
            result += "(";

            for (int j = 0; j < n; j++) {
                if (g) {
                    result += " ∧ ";
                } else {
                    g = true; // первый операнд
                }

                int v = (i >> (n - j - 1)) & 1; // j-й разряд кортежа i

                if (v == 0) {
                    result += "¬";
                }

                result += x[j];
            }

            result += ")";
        }
    }

    return result;
}

string buildSKNF(const vector<string>& x, const vector<int>& F) {
    bool f = false; // левый операнд конъюнкции
    string result;
    int n = x.size();
    int total = 1 << n; // 2^n

    for (int i = 0; i < total; i++) {
		// СКНФ строится по нулям функции
        if (F[i] == 0) {
            if (f) {
                result += " ∧ ";
            } else {
                f = true; // первое выражение
            }

            bool g = false; // первый операнд дизъюнкции
            result += "(";

            for (int j = 0; j < n; j++) {
                if (g) {
                    result += " ∨ ";
                } else {
                    g = true;
                }

                int v = (i >> (n - j - 1)) & 1; // j-й разряд кортежа i

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