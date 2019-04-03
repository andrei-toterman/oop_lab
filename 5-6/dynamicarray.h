#pragma once

template <typename T>
class DynamicArray {
    private:
        int cap;
        int len;
        T* elems;
        void resize();
    public:
        DynamicArray(int cap = 10);
        ~DynamicArray();

        int size() const { return this->len; }
        T* begin() const { return this->elems; }
        T* end() const { return this->elems + this->len; }
        T& operator[] (int index) const;

        void add(const T& elem);
        int find(const T& elem) const;
        void remove(int index);
        void update(int index, const T& new_elem);
};

#include "dynamicarray.tpp"
