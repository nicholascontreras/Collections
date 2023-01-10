#pragma once

#include <initializer_list>
#include <functional>

#include "Collection.h"

template <class T>
class ArrayList : public Collection<T> {
public:
    inline ArrayList() : size(0), backingArrayLength(32) {
        backingArray = new T[backingArrayLength];
    }

    inline ArrayList(std::initializer_list<T> items) : ArrayList() {
        for(T curItem : items) {
            add(curItem);
        }
    }

    inline ArrayList(const ArrayList<T>& other) : ArrayList() {
        for(T curItem : other) {
            add(curItem);
        }
    };
    ArrayList& operator=(const ArrayList& other) noexcept = delete;
    ArrayList(ArrayList&& other) = delete;
    ArrayList& operator=(ArrayList&& other) noexcept = delete;

    inline ~ArrayList() {
        delete[] backingArray;
    }

    inline void add(T item) {
        rotateSyncKey();
        if(size == backingArrayLength) {
            expandBackingArray();
        }

        backingArray[size++] = item;
    }

    inline void insert(T item, int index) {
        if(!indexInRange(index) && index != size) {
            throw IndexOutOfBoundsException("Invalid index: (" + std::to_string(index) +
                ") for inserting into ArrayList of size: (" + std::to_string(size) + ")");
        }

        rotateSyncKey();
        if(size == backingArrayLength) {
            expandBackingArray();
        }

        for(int i = size - 1; i >= index; i--) {
            backingArray[i + 1] = backingArray[i];
        }
        backingArray[index] = item;
        size++;
    }

    inline void set(T item, int index) {
        if(!indexInRange(index)) {
            throw IndexOutOfBoundsException("Invalid index: (" + std::to_string(index) +
                ") for setting value of ArrayList of size: (" + std::to_string(size) + ")");
        }

        rotateSyncKey();
        backingArray[index] = item;
    }

    inline const T& get(int index) const {
        if(!indexInRange(index)) {
            throw IndexOutOfBoundsException("Invalid index: (" + std::to_string(index) +
                ") for accessing element of ArrayList of size: (" + std::to_string(getSize()) + ")");
        }
        return backingArray[index];
    }

    inline T& get(int index) {
        if(!indexInRange(index)) {
            throw IndexOutOfBoundsException("Invalid index: (" + std::to_string(index) +
                ") for accessing element of ArrayList of size: (" + std::to_string(getSize()) + ")");
        }
        return backingArray[index];
    }

    virtual inline int getSize() const { return size; };

    inline bool operator==(const ArrayList<T>& other) const {
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

    inline bool operator!=(const ArrayList<T>& other) const {
        return !operator==(other);
    }

    friend inline void swap(ArrayList<T>& lhs, ArrayList<T>& rhs) {
        using std::swap;
        swap(lhs.size, rhs.size);
        swap(lhs.backingArray, rhs.backingArray);
        swap(lhs.backingArrayLength, rhs.backingArrayLength);
    }

    template <class U>
    class VectorIterator : public Iterator<U>::IteratorImpl {
    public:
        inline VectorIterator(const Collection<T>& collection, int index, std::function<U(int)> valueFunc, std::function<unsigned int()> syncKeyFunc) :
            IteratorImpl(&collection),
            collection(collection),
            index(index), 
            valueFunc(valueFunc), 
            syncKeyFunc(syncKeyFunc),
            startingSyncKey(syncKeyFunc()) {};

        virtual inline U get() {
            if(syncKeyFunc() == startingSyncKey) {
                return valueFunc(index);
            } else {
                throw ConcurrentModificationException("Detected modification of underlying ArrayList during Iterator get!");
            }
        }
        virtual inline void next() {
            if(syncKeyFunc() == startingSyncKey) {
                index++;
            } else {
                throw ConcurrentModificationException("Detected modification of underlying ArrayList during Iterator next!");
            }
        }
        virtual inline bool atEnd() {
            return index == collection.getSize();
        }
    private:
        const Collection<T>& collection;
        int index;
        std::function<U(int)> valueFunc;
        std::function<unsigned int()> syncKeyFunc;
        unsigned int startingSyncKey;
    };

    inline virtual Iterator<T&> begin() {
        return Iterator<T&>(new VectorIterator<T&>(*this, 0, 
            [&](int index)->T& { return get(index); }, 
            std::bind(&ArrayList::getSyncKey, this)));
    }

    inline virtual Iterator<T&> end() {
        return Iterator<T&>(new VectorIterator<T&>(*this, size,
            [&](int index)->T& { return get(index); },
            std::bind(&ArrayList::getSyncKey, this)));
    }

    inline virtual Iterator<const T&> begin() const {
        return Iterator<const T&>(new VectorIterator<const T&>(*this, 0,
            [&](int index)->const T& { return get(index); },
            std::bind(&ArrayList::getSyncKey, this)));
    }
    inline virtual Iterator<const T&> end() const {
        return Iterator<const T&>(new VectorIterator<const T&>(*this, size,
            [&](int index)->const T& { return get(index); },
            std::bind(&ArrayList::getSyncKey, this)));
    }
private:
    int size;

    T* backingArray;
    int backingArrayLength;

    inline void expandBackingArray() {
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