#pragma once
#include <string>
#include <stdexcept>

template <typename T>
class DynamicVector {
    protected:
        // capacity of the DynamicVector
        int cap;
        // current number of elements in the DynamicVector
        int len;
        // pointer to a block of memory that contains elements in sequential order
        T* elems;
        // doubles the capacity of the DynamicVector
        void resize();
    public:
        /*
         * default constructor
         * input: _cap: the initial capacity of the DynamicVector
         */
        DynamicVector(int _cap = 3);
        /*
         * copy constructor
         * input: other: reference to another DynamicVector from which to copy
         */
        DynamicVector(const DynamicVector<T>& other);
        /*
         * default destructor
         * deallocates the memory pointed by elems
         */
        ~DynamicVector();

        // get the current capacity
        int capacity() const { return this->cap; }
        // get the current number of elements
        int size() const { return this->len; }
        // returns a pointer to the beginning of the list of elements
        T* begin() const { return this->elems; }
        // returns a pointer past the end of the list of elements
        T* end() const { return this->elems + this->len; }

        /*
         * adds a given T element to the list of elements
         * input: elem: reference to the element to be added
         */
        void add(const T& elem);
        /*
         * removes the element at the given position in the list
         * input: index: the position of the element to be deleted
         * throws: out_of_range if index is negatie or greater or equal than the number of elements
         */
        void remove(int index);

        /*
         * returns a reference to the element at the given position from the list of elements
         * input: index: the position of the element to be returnes
         * throws: out_of_range if index is negatie or greater or equal than the number of elements
         */
        T& operator[] (int index) const;
        /*
         * assigns to an existing DynamicVecotr another Dynamic vector by deep-copying the elements
         * other: reference to an existing DynamicVector from which to copy
         * return: the newly modified DynamicVector
         */
        DynamicVector<T>& operator=(const DynamicVector<T>& other);
        /*
         * adds a given element to the DynamicVector
         * input: elem: reference to element to be added
         * output: the Dynamic vector to which the element has been added
         */
        DynamicVector<T>& operator+(const T& elem);
};

template <typename T>
DynamicVector<T>::DynamicVector(int _cap) {
    this->cap = _cap;
    this->len = 0;
    this->elems = new T[this->cap];
}

template<typename T>
DynamicVector<T>::DynamicVector(const DynamicVector<T>& other) {
    this->cap = other.cap;
    this->len = 0;
    this->elems = new T[this->cap];
    for (T elem : other)
        this->elems[this->len++] = elem;
}

template<typename T>
DynamicVector<T>::~DynamicVector() {
    delete[] this->elems;
}

template<typename T>
T& DynamicVector<T>::operator[](int index) const {
    if (index < 0 || index >= this->len)
        throw std::out_of_range("invalid index");
    return this->elems[index];
}

template<typename T>
DynamicVector<T>& DynamicVector<T>::operator=(const DynamicVector<T>& other) {
    if (this == &other) return *this;
    this->cap = other.cap;
    this->len = 0;
    delete[] this->elems;
    this->elems = new T[this->cap];
    for (T elem : other)
        this->elems[this->len++] = elem;
    return *this;
}

template<typename T>
DynamicVector<T>& DynamicVector<T>::operator+(const T& elem) {
    this->add(elem);
    return *this;
}

template <typename T>
DynamicVector<T>& operator+(const T& elem, DynamicVector<T>& vector) {
    vector.add(elem);
    return vector;
}

template<typename T>
void DynamicVector<T>::add(const T& elem) {
    if (this->len == this->cap) this->resize();
    this->elems[this->len++] = elem;
}

template<typename T>
void DynamicVector<T>::remove(int index) {
    if (index < 0 || index >= this->len)
        throw std::out_of_range("invalid index");
    for (int i = index; i < this->len - 1; ++i)
        this->elems[i] = this->elems[i + 1];
    this->len--;
}

template<typename T>
void DynamicVector<T>::resize() {
    this->cap *= 2;
    T* new_elems = new T[this->cap];
    for (int i = 0; i < this->len; ++i)
        new_elems[i] = this->elems[i];
    delete[] this->elems;
    this->elems = new_elems;
}

