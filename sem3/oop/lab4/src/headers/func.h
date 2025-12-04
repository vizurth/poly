#pragma once
#include <string>
#include <iostream>
#include <stack>
#include <queue>
#include <type_traits> // Для std::is_pointer_v

template <typename T, typename Container>
auto adapter_peek(const std::stack<T, Container>& adapter) -> decltype(adapter.top());

template <typename T, typename Container, typename Compare>
auto adapter_peek(const std::priority_queue<T, Container, Compare>& adapter) -> decltype(adapter.top());

template <typename T, typename Container>
auto adapter_peek(const std::queue<T, Container>& adapter) -> decltype(adapter.front());


template <typename T>
void printSQPQ(T adapter, const std::string& name);

template <typename T>
void printContainer(const T& cont, std::string s);

template <typename T>
void printSQPQ(T  sqpq, const std::string& s);

// Включаем реализацию шаблона
#include "../tpl/func.tpp"