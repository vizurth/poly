#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

using namespace std;

// узел BDD графа
struct BDDNode {
    int id; // id - ноды для более удобного создания
    string var; // имя переменной в ноде
    int zero; // пунктирная линия
    int one; // сплошная линия
    bool isSheet; // является ли нода листом
    
    BDDNode() : id(-1), var(""), zero(-1), one(-1), isSheet(false) {}
    
    BDDNode(int _id, string _var, int _zero, int _one, bool _term)
        : id(_id), var(_var), zero(_zero), one(_one), isSheet(_term) {}
};

// класс для хранения и работы с BDD графом
class BDDGraph {
private:
    map<int, BDDNode> nodes; // нода графа по id
    int rootId; // id корневой ноды
    
public:
    BDDGraph() : rootId(-1) {}
    
    // ф-ция для добавления ноды
    void addNode(int id, string var, int zero, int one, bool isSheet) {
        nodes[id] = BDDNode(id, var, zero, one, isSheet);
    }
       
    // установка корневой ноды
    void setRoot(int id) {
        rootId = id;
    }
    
    // построение BDD
    void buildFromDiagram() {
        // листья
        addNode(0, "0", -1, -1, true);   // 0
        addNode(1, "1", -1, -1, true);   // 1
		
		// внутренние ноды
        addNode(2, "x3", 1, 0, false);
        addNode(3, "x3", 0, 1, false);
        addNode(4, "x4", 2, 3, false);
        addNode(5, "x4", 3, 1, false);
        addNode(6, "x2", 4, 5, false);
        addNode(7, "x1", 2, 6, false);
        
		// корневая нода
        setRoot(7);
    }
    
    // вычисление значения функции для заданных переменных path (x1,x2,x3,x4)
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
        
        // если лист то возвращаем значение
        if (node.isSheet) {
            path.push_back("Достигнут лист: " + node.var);
            return stoi(node.var);
        }
        
        // значение переменной в текущем узле
        int varValue = values[node.var];
        
        // записываем путь
        string direction = (varValue == 0) ? "(пунктир)" : "(сплошная)";
        path.push_back("Узел " + to_string(nodeId) + " (" + node.var + 
                      " = " + to_string(varValue) + ") -> " + direction);
        
        // переходим к следующему узлу
        if (varValue == 0) {
            return evaluateNode(node.zero, values, path);
        } else {
            return evaluateNode(node.one, values, path);
        }
    }
    
public:
    // вывод графа
    void printGraph() {
        cout << "\n=== Структура BDD графа ===" << endl;
        cout << "Корневой узел: " << rootId << endl << endl;
        
        for (map<int, BDDNode>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
            BDDNode& node = it->second;
            cout << "Узел " << node.id << ": " << node.var;
            
            if (node.isSheet) {
                cout << " (лист)" << endl;
            } else {
                cout << " -> zero: " << node.zero 
                     << ", one: " << node.one << endl;
            }
        }
        cout << "========================" << endl << endl;
    }
};