#pragma once

#include <initializer_list>
#include <functional>

#include "Collection.h"
#include "Iterator.h"

template <class T>
class ArrayList : public Collection<T> {
public:
    ArrayList() : size(0), backingArrayLength(32) {
        backingArray = new T[backingArrayLength];
    }

    ArrayList(std::initializer_list<T> items) : ArrayList() {
        for(T curItem : items) {
            add(curItem);
        }
    }

    ArrayList(const ArrayList<T>& other) : ArrayList() {
        for(T curItem : other) {
            add(curItem);
        }
    }
    ArrayList& operator=(const ArrayList& other) {
        // Manually avoid the obvious optimization of taking the rhs by value
        // This ensures r-value assignment is not ambiguous between this operator and move-assignment
        // Allows testing all of the copy and move operations explicitly
        ArrayList temp(other);
        swap(*this, temp);
        return *this;
    }
    ArrayList(ArrayList&& other) noexcept : ArrayList() {
        swap(*this, other);
    }
    ArrayList& operator=(ArrayList&& other) noexcept {
        swap(*this, other);
        return *this;
    }

    ~ArrayList() {
        delete[] backingArray;
    }

    void add(T item) {
        this->rotateSyncKey();
        if(size == backingArrayLength) {
            expandBackingArray();
        }

        backingArray[size++] = item;
    }

    void insert(T item, int index) {
        if(!indexInRange(index) && index != size) {
            throw IndexOutOfBoundsException("Invalid index: (" + std::to_string(index) +
                ") for inserting into ArrayList of size: (" + std::to_string(size) + ")");
        }

        this->rotateSyncKey();
        if(size == backingArrayLength) {
            expandBackingArray();
        }

        for(int i = size - 1; i >= index; i--) {
            backingArray[i + 1] = backingArray[i];
        }
        backingArray[index] = item;
        size++;
    }

    void set(T item, int index) {
        if(!indexInRange(index)) {
            throw IndexOutOfBoundsException("Invalid index: (" + std::to_string(index) +
                ") for setting value of ArrayList of size: (" + std::to_string(size) + ")");
        }

        this->rotateSyncKey();
        backingArray[index] = item;
    }

    const T& get(int index) const {
        if(!indexInRange(index)) {
            throw IndexOutOfBoundsException("Invalid index: (" + std::to_string(index) +
                ") for accessing element of ArrayList of size: (" + std::to_string(getSize()) + ")");
        }
        return backingArray[index];
    }

    T& get(int index) {
        if(!indexInRange(index)) {
            throw IndexOutOfBoundsException("Invalid index: (" + std::to_string(index) +
                ") for accessing element of ArrayList of size: (" + std::to_string(getSize()) + ")");
        }
        return backingArray[index];
    }

    inline virtual int getSize() const { return size; };

    bool operator==(const ArrayList<T>& other) const {
        if(getSize() == other.getSize()) {
            for(int i = 0; i < getSize(); i++) {
                if(get(i) != other.get(i)) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    bool operator!=(const ArrayList<T>& other) const {
        return !operator==(other);
    }

    friend void swap(ArrayList<T>& lhs, ArrayList<T>& rhs) {
        using std::swap;
        swap(lhs.size, rhs.size);
        swap(lhs.backingArray, rhs.backingArray);
        swap(lhs.backingArrayLength, rhs.backingArrayLength);
    }

    template <class U>
    class VectorIterator : public Iterator<U>::IteratorImpl {
    public:
        VectorIterator(int index, std::function<U(int)> valueFunc) :
            index(index), 
            valueFunc(valueFunc) {};

        U get() override {
            return valueFunc(index);
        }
        void next() override {
            index++;
        }
        bool samePosition(const Iterator<U>::IteratorImpl& other) override {
            return index == other.index;
        }
    private:
        int index;
        std::function<U(int)> valueFunc;
    };

    Iterator<T&> begin() override {
        return Iterator<T&>(*this, new VectorIterator<T&>(0, 
            [&](int index)->T& { return get(index); }));
    }

    Iterator<T&> end() override {
        return Iterator<T&>(*this, new VectorIterator<T&>(size,
            [&](int index)->T& { return get(index); }));
    }

    Iterator<const T&> begin() const override {
        return Iterator<const T&>(*this, new VectorIterator<const T&>(0,
            [&](int index)->const T& { return get(index); }));
    }
    Iterator<const T&> end() const override {
        return Iterator<const T&>(*this, new VectorIterator<const T&>(size,
            [&](int index)->const T& { return get(index); }));
    }
private:
    int size;

    T* backingArray;
    int backingArrayLength;

    void expandBackingArray() {
        T* newBackingArray = new T[(size_t)backingArrayLength * 2];
        for(int i = 0; i < size; i++) {
            newBackingArray[i] = backingArray[i];
        }
        delete[] backingArray;
        backingArray = newBackingArray;
        backingArrayLength *= 2;
    }

    inline bool indexInRange(int index) const {
        return index >= 0 && index < size;
    }
};