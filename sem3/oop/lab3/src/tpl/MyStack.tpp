template<class T, int N>
MyStack<T, N>::MyStack() : size(-1) {}

template<class T, int N>
void MyStack<T, N>::Push(const T& elem){
  if (size >= N - 1) {
      throw StackOverflow();
  }
  arr[++size] = elem;
}

template<class T, int N>
T MyStack<T, N>::Pop(){
  if (size < 0){
      throw StackUnderflow();
  }
  return arr[size--];
}

template<class T, int N>
int MyStack<T, N>::GetSize(){
  return size + 1;
}

template<class T, int N>
int MyStack<T, N>::Capacity(){
  return N;
}

template<class T, int N>
T& MyStack<T, N>::operator[](int index){
  if (index < 0 || index > size){
      throw StackOutOfRange(index);
  }
  return arr[index];
}

template<class T, int N>
const T MyStack<T, N>::operator[](int index) const{
  if (index < 0 || index > size){
      throw StackOutOfRange(index);
  }
  return arr[index];
}
