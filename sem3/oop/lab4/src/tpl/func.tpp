#include <stack>
#include <queue>
#include <iostream>
#include <type_traits>

using namespace std;

template <typename T>
void printContainer(const T& cont, std::string s) {
	std::cout << "\n\n" << s << "\n";
  for (const auto& item : cont) {
      std::cout << item << " ";
  }
  std::cout << std::endl;
}

template <typename T, typename Container>
auto adapter_peek(const std::stack<T, Container>& adapter) -> decltype(adapter.top()) {
    return adapter.top();
}

template <typename T, typename Container, typename Compare>
auto adapter_peek(const std::priority_queue<T, Container, Compare>& adapter) -> decltype(adapter.top()) {
    return adapter.top();
}

template <typename T, typename Container>
auto adapter_peek(const std::queue<T, Container>& adapter) -> decltype(adapter.front()) {
    return adapter.front();
}

template <typename T>
void printAdapter(T adapter, const std::string& name) {
    T temp = adapter; 

    std::cout << "\n\t" << name << " Content:\n";

    using value_type = typename T::value_type;

    while (!temp.empty()) {
        const auto& element = adapter_peek(temp); 
        if constexpr (std::is_pointer_v<value_type>) { 
            if (element) {
                std::cout << "Value: " << *element << ", Address: " << element << std::endl; 
            } else {
                std::cout << "NULL" << std::endl;
            }
        } else {
            std::cout << element << std::endl; 
        }
        temp.pop();
    }
}
template <typename T>
void printMap(const T& container) {
    for (const auto& pair : container) {
        std::cout << "{" << pair.first << ": " << pair.second << "} ";
    }
    std::cout << std::endl;
}

template<typename T>
void print(const T& item) {
    std::cout << item << " ";
}

