#include <stack>
#include <queue>
#include <iostream>
#include <type_traits>

using namespace std;

template <class T> void printContainer(T& v, string s)
{
	cout<<"\n\n\t"<<s<<"  # Sequence:\n";
	
	// Итератор любого контейнера
	typename T::iterator p;
	int i;

	for (p = v.begin(), i=0;  p != v.end();  p++, i++)
		cout << endl << i+1 <<". "<< *p;
	cout << '\n';
}

template <typename T>
void printSQPQ(T  sqpq, const string& s){
	cout<<"\n\n\t"<<s<<"  # Sequence:\n";
	while(!sqpq.empty()){
		if constexpr (std::is_same_v<T, std::queue<typename T::value_type>>) { // если 
			std::cout << sqpq.front() << " ";
		}else{
			cout<< sqpq.top()<<" "; 
		}
		sqpq.pop();
	}
	cout<<"\n";
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

