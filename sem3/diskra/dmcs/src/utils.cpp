#include "utils.h"
#include "constants.h"
#include <iostream>

std::map<char, char> NEXT_SYMBOL_MAP;
std::map<char, char> SYMBOL_NEGATION_MAP;
std::map<std::tuple<char, char, char>, std::pair<char, char>> ADDITION_TABLE;

char symbolic_add_simple(char char1, char char2) {
    if (char1 == get_additive_unit()) return char2;
    if (char2 == get_additive_unit()) return char1;
    
    char result = char1;
    char counter = get_additive_unit();
    
    while (counter != char2) {
        result = NEXT_SYMBOL_MAP.at(result);
        counter = NEXT_SYMBOL_MAP.at(counter);
    }
    
    return result;
}
void print_addition_table() {
    std::cout << "=== ADDITION TABLE (c1, c2, carry_in) -> (sum, carry_out) ===\n";

    for (char c1 : SYMBOLS) {
        for (char c2 : SYMBOLS) {
            for (char carry_in : SYMBOLS) {
                auto key = std::make_tuple(c1, c2, carry_in);
                auto it = ADDITION_TABLE.find(key);
                if (it == ADDITION_TABLE.end()) continue;

                char sum = it->second.first;
                char carry_out = it->second.second;

                std::cout << " (" << c1 << ", " << c2 << ", " << carry_in << ")"
                          << " -> "
                          << "(" << sum << ", " << carry_out << ")"
                          << "\n";
            }
        }
    }
}


void init_symbols_maps() {
    // Инициализируем правило +1 через явное задание цикла
    NEXT_SYMBOL_MAP['0'] = '1';
    NEXT_SYMBOL_MAP['1'] = '2'; 
    NEXT_SYMBOL_MAP['2'] = '3';
    NEXT_SYMBOL_MAP['3'] = '4';
    NEXT_SYMBOL_MAP['4'] = '5';
    NEXT_SYMBOL_MAP['5'] = '6';
    NEXT_SYMBOL_MAP['6'] = '7';
    NEXT_SYMBOL_MAP['7'] = '0';  // Замыкаем цикл
    
    // Инициализируем таблицу отрицаний (x + neg(x) = 'a')
    SYMBOL_NEGATION_MAP['0'] = '0';
    SYMBOL_NEGATION_MAP['1'] = '7';
    SYMBOL_NEGATION_MAP['2'] = '6';
    SYMBOL_NEGATION_MAP['3'] = '5';
    SYMBOL_NEGATION_MAP['4'] = '4';  // Сам себе обратный
    SYMBOL_NEGATION_MAP['5'] = '3';
    SYMBOL_NEGATION_MAP['6'] = '2';
    SYMBOL_NEGATION_MAP['7'] = '1';
    
    // Инициализируем таблицу сложения с переносами
    for (char c1 : SYMBOLS) {
        for (char c2 : SYMBOLS) {
            for (char carry_in : SYMBOLS) {
                // Складываем три символа
                char sum = symbolic_add_simple(symbolic_add_simple(c1, c2), carry_in);
                
                // Определяем перенос
                char carry_out = get_additive_unit();
                
                // Эмулируем сложение в десятичной системе для определения переноса
                int total = 0;
                char current = get_additive_unit();
                
                // Считаем c1
                while (current != c1) {
                    total++;
                    current = NEXT_SYMBOL_MAP.at(current);
                }
                
                // Считаем c2
                current = get_additive_unit();
                while (current != c2) {
                    total++;
                    current = NEXT_SYMBOL_MAP.at(current);
                }
                
                // Считаем carry_in
                current = get_additive_unit();
                while (current != carry_in) {
                    total++;
                    current = NEXT_SYMBOL_MAP.at(current);
                }
                
                // Если сумма превышает MOD-1, есть перенос
                if (total >= MOD) {
                    carry_out = get_multiplicative_unit();
                }
                
                ADDITION_TABLE[std::make_tuple(c1, c2, carry_in)] = std::make_pair(sum, carry_out);
            }
        }
    }
}

bool is_valid_digit(char c) {
    return NEXT_SYMBOL_MAP.count(c);
}

char get_additive_unit() {
    return '0';
}

char get_multiplicative_unit() {
    return '1';
}

std::string get_min_value_str() {
    return "-" + std::string(MAX_DIGITS, 'f');
}

std::string get_max_value_str() {
    return std::string(MAX_DIGITS, 'f');
}

int compare_chars(char c1, char c2) {
    if (c1 == c2) return 0;
    
    // Проходим по циклу, чтобы определить порядок
    char current = get_additive_unit();
    while (true) {
        if (current == c1) return -1;
        if (current == c2) return 1;
        current = NEXT_SYMBOL_MAP.at(current);
        if (current == get_additive_unit()) break; // Прошли полный цикл
    }
    return 0; // Не должно случиться
}