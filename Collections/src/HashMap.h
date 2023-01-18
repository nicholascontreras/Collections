#pragma once

#include "ArrayList.h"

template<class A, class B>
struct KeyValuePair { 

    KeyValuePair() : key(A()), value(B()) {};
    KeyValuePair(A key, B value) : key(key), value(value) {};

    const A key;
    B value;
};

template<class T, class U>
class HashMap : public TemplatedCollection<KeyValuePair<T, U>> {
public:
    HashMap() : loadFactor(0.75) {}

    Iterator<KeyValuePair<T, U>&> begin() override {
        std::function<KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);  
        };
        return Iterator<KeyValuePair<T, U>&>(*this, new HashMapIterator<KeyValuePair<T, U>&>(0, valueFunc));
    }
    Iterator<KeyValuePair<T, U>&> end() {
        std::function<KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);
        };
        return Iterator<KeyValuePair<T, U>&>(*this, new HashMapIterator<KeyValuePair<T, U>&>(buckets.getSize(), valueFunc));
    }

    Iterator<const KeyValuePair<T, U>&> begin() const {
        std::function<const KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->const KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);
        };
        return Iterator<const KeyValuePair<T, U>&>(*this, new HashMapIterator<const KeyValuePair<T, U>&>(0, valueFunc));
    }
    Iterator<const KeyValuePair<T, U>&> end() const {
        std::function<const KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->const KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);
        };
        return Iterator<const KeyValuePair<T, U>&>(*this, new HashMapIterator<const KeyValuePair<T, U>&>(buckets.getSize(), valueFunc));
    }

    virtual int getSize() const {
        return numEntries;
    }
private:
    template <class V>
    class HashMapIterator : public Iterator<V>::IteratorImpl {
    public:
        HashMapIterator(int bucket, std::function<V(int, int)> valueFunc) :
            curBucket(bucket),
            curIndexInBucket(0),
            valueFunc(valueFunc) {};

        V get() override {
            return valueFunc(curBucket, curIndexInBucket);
        }
        void next() override {
            curIndexInBucket++;
            if(curIndexInBucket == hashMap.buckets.get(curBucket).getSize()) {
                curIndexInBucket = 0;
                curBucket++;
            }
        }
        bool samePosition(const Iterator<V>::IteratorImpl& o) const override {
            const HashMapIterator& other = dynamic_cast<const HashMapIterator&>(o);
            return curBucket == other.curBucket && curIndexInBucket == other.curIndexInBucket;
        }

    private:
        int curBucket, curIndexInBucket;

        std::function<V(int,int)> valueFunc;
    };


    int numEntries;
    ArrayList<ArrayList<KeyValuePair<T, U>>> buckets;
    double loadFactor;
};