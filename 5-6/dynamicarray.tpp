#include "dynamicarray.h"

template <typename T>
DynamicArray<T>::DynamicArray(int cap) {
    this->cap = cap;
    this->len = 0;
    this->elems = new T[this->cap];
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] this->elems;
}

template<typename T>
T& DynamicArray<T>::operator[](int index) const {
    return this->elems[index];
}

template<typename T>
void DynamicArray<T>::add(const T& elem) {
    if (this->len == this->cap) this->resize();
    this->elems[this->len++] = elem;
}

template<typename T>
int DynamicArray<T>::find(const T& elem) const {
    for (int i = 0; i < this->len; ++i)
        if (this->elems[i] == elem)
            return i;
    return -1;
}

template<typename T>
void DynamicArray<T>::remove(int index) {
    for (int i = --this->len; i > index; --i)
        this->elems[i - 1] = this->elems[i];
}

template<typename T>
void DynamicArray<T>::update(int index, const T& new_elem) {
    this->elems[index] = new_elem;
}

template<typename T>
void DynamicArray<T>::resize() {
    this->cap *= 2;
    T* new_elems = new T[this->cap];
    for (int i = 0; i < this->len; ++i)
        new_elems[i] = this->elems[i];
    delete[] this->elems;
    this->elems = new_elems;
}
