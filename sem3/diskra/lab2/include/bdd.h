#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

using namespace std;

// Узел BDD графа
struct BDDNode {
    int id;                    // Уникальный идентификатор узла
    string variable;           // Имя переменной или терминал ("0", "1")
    int low;                   // ID узла для перехода при значении 0 (пунктирная линия)
    int high;                  // ID узла для перехода при значении 1 (сплошная линия)
    bool isTerminal;           // Является ли узел терминальным
    
    BDDNode() : id(-1), variable(""), low(-1), high(-1), isTerminal(false) {}
    
    BDDNode(int _id, string _var, int _low, int _high, bool _term)
        : id(_id), variable(_var), low(_low), high(_high), isTerminal(_term) {}
};

// Класс для хранения и работы с BDD графом
class BDDGraph {
private:
    map<int, BDDNode> nodes;   // Хранилище всех узлов графа по ID
    int rootId;                // ID корневого узла
    
public:
    BDDGraph() : rootId(-1) {}
    
    // Добавление узла в граф
    void addNode(int id, string variable, int low, int high, bool isTerminal) {
        nodes[id] = BDDNode(id, variable, low, high, isTerminal);
    }
    
    // Установка корневого узла
    void setRoot(int id) {
        rootId = id;
    }
    
    // Построение BDD из вашей диаграммы
    void buildFromDiagram() {
        // листья
        addNode(0, "0", -1, -1, true);   // 0
        addNode(1, "1", -1, -1, true);   // 1
        
        addNode(2, "x3", 1, 0, false);
        
        addNode(3, "x3", 0, 1, false);
        
        addNode(4, "x4", 2, 3, false);
        
        addNode(5, "x4", 3, 1, false);
        
        addNode(6, "x2", 4, 5, false);
        
        addNode(7, "x1", 2, 6, false);
        
        setRoot(7);
    }
    
    // Вычисление значения BDD для заданных значений переменных
    int evaluate(map<string, int>& values, vector<string>& path) {
        if (rootId == -1) {
            cerr << "Ошибка: граф не инициализирован!" << endl;
            return -1;
        }
        return evaluateNode(rootId, values, path);
    }
    
private:
    // Рекурсивное вычисление от текущего узла
    int evaluateNode(int nodeId, map<string, int>& values, vector<string>& path) {
        BDDNode& node = nodes[nodeId];
        
        // Если терминальный узел - возвращаем его значение
        if (node.isTerminal) {
            path.push_back("Достигнут терминал: " + node.variable);
            return stoi(node.variable);
        }
        
        // Получаем значение текущей переменной
        int varValue = values[node.variable];
        
        // Записываем путь
        string direction = (varValue == 0) ? "low (пунктир)" : "high (сплошная)";
        path.push_back("Узел " + to_string(nodeId) + " (" + node.variable + 
                      " = " + to_string(varValue) + ") -> " + direction);
        
        // Переходим по соответствующей ветви
        if (varValue == 0) {
            return evaluateNode(node.low, values, path);
        } else {
            return evaluateNode(node.high, values, path);
        }
    }
    
public:
    // Вывод структуры графа
    void printGraph() {
        cout << "\n=== Структура BDD графа ===" << endl;
        cout << "Корневой узел: " << rootId << endl << endl;
        
        for (map<int, BDDNode>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
            BDDNode& node = it->second;
            cout << "Узел " << node.id << ": " << node.variable;
            
            if (node.isTerminal) {
                cout << " (терминал)" << endl;
            } else {
                cout << " -> low: " << node.low 
                     << ", high: " << node.high << endl;
            }
        }
        cout << "========================" << endl << endl;
    }
};