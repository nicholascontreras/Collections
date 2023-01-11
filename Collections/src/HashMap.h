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
class HashMap : public Collection<KeyValuePair<T, U>> {
public:
    HashMap() : loadFactor(0.75) {}

    Collection<KeyValuePair<T, U>>::Iterator<KeyValuePair<T, U>&> begin() override {
        std::function<KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);  
        };
        return Collection<KeyValuePair<T, U>>::Iterator<KeyValuePair<T, U>&>(new HashMapIterator<KeyValuePair<T, U>&>(*this, 0, valueFunc));
    }
    virtual Iterator<KeyValuePair<T, U>&> end() {
        std::function<KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);
        };
        return Iterator<KeyValuePair<T, U>&>(new HashMapIterator<KeyValuePair<T, U>&>(*this, buckets.getSize(), valueFunc));
    }

    virtual Iterator<const KeyValuePair<T, U>&> begin() const {
        std::function<const KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->const KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);
        };
        return Iterator<const KeyValuePair<T, U>&>(new HashMapIterator<const KeyValuePair<T, U>&>(*this, 0, valueFunc));
    }
    virtual Iterator<const KeyValuePair<T, U>&> end() const {
        std::function<const KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->const KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);
        };
        return Iterator<const KeyValuePair<T, U>&>(new HashMapIterator<const KeyValuePair<T, U>&>(*this, buckets.getSize(), valueFunc));
    }

    virtual int getSize() const {
        return numEntries;
    }
private:
    template <class V>
    class HashMapIterator : public Iterator<V>::IteratorImpl {
    public:
        HashMapIterator(const HashMap& hashMap, int bucket, std::function<V(int, int)> valueFunc) :
            IteratorImpl(&hashMap),
            hashMap(hashMap),
            curBucket(bucket),
            curIndexInBucket(0),
            valueFunc(valueFunc) {};

        virtual V get() {
            return valueFunc(curBucket, curIndexInBucket);
        }
        virtual void next() {
            curIndexInBucket++;
            if(curIndexInBucket == hashMap.buckets.get(curBucket).getSize()) {
                curIndexInBucket = 0;
                curBucket++;
            }
        }
        virtual bool atEnd() {
            return curBucket == hashMap.buckets.getSize();
        }

    private:
        const HashMap& hashMap;
        int curBucket, curIndexInBucket;

        std::function<V(int,int)> valueFunc;
    };


    int numEntries;
    ArrayList<ArrayList<KeyValuePair<T, U>>> buckets;
    double loadFactor;
};