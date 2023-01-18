#pragma once

#include "ArrayList.h"

template<class A, class B>
struct KeyValuePair { 

    KeyValuePair() : key(A()), value(B()) {};
    KeyValuePair(A key, B value) : key(key), value(value) {};

    A key;
    B value;
};

template<class T, class U>
class HashMap : public TemplatedCollection<KeyValuePair<T, U>> {
public:
    HashMap(std::function<int(const T)> hashFunction) : 
        numEntries(0),
        hashFunction(hashFunction), 
        loadFactor(0.75) {

        for(int i = 0; i < 16; i++) {
            buckets.add(ArrayList<KeyValuePair<T, U>>());
        }
    }

    bool containsKey(T key) {
        int bucket = hashKey(key);
        return keyPositionInBucket(key, bucket) != -1;
    }

    void put(T key, U value) {
        this->rotateSyncKey();

        if(numEntries + 1 >= buckets.getSize() * loadFactor) {
            expandAndRehash();
        }

        int bucket = hashKey(key);
        if(containsKey(key)) {
            buckets.get(bucket).get(keyPositionInBucket(key, bucket)).value = value;
        } else {
            buckets.get(bucket).add(KeyValuePair<T, U>(key, value));
            numEntries++;
        }
    }

    U get(T key) {
        if(!containsKey(key)) {
            throw NoSuchItemException("The given key does not map to any entry!");
        } else {
            int bucket = hashKey(key);
            return buckets.get(bucket).get(keyPositionInBucket(key, bucket)).value;
        }
    }

    Iterator<KeyValuePair<T, U>&> begin() override {
        std::function<KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);  
        };
        return Iterator<KeyValuePair<T, U>&>(*this, new HashMapIterator<KeyValuePair<T, U>&>(0, valueFunc, *this));
    }
    Iterator<KeyValuePair<T, U>&> end() {
        std::function<KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);
        };
        return Iterator<KeyValuePair<T, U>&>(*this, new HashMapIterator<KeyValuePair<T, U>&>(buckets.getSize(), valueFunc, *this));
    }

    Iterator<const KeyValuePair<T, U>&> begin() const {
        std::function<const KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->const KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);
        };
        return Iterator<const KeyValuePair<T, U>&>(*this, new HashMapIterator<const KeyValuePair<T, U>&>(0, valueFunc, *this));
    }
    Iterator<const KeyValuePair<T, U>&> end() const {
        std::function<const KeyValuePair<T, U>& (int, int)> valueFunc = [this](int curBucket, int curIndexInBucket)->const KeyValuePair<T, U>& {
            return buckets.get(curBucket).get(curIndexInBucket);
        };
        return Iterator<const KeyValuePair<T, U>&>(*this, new HashMapIterator<const KeyValuePair<T, U>&>(buckets.getSize(), valueFunc, *this));
    }

    virtual int getSize() const {
        return numEntries;
    }
private:
    template <class V>
    class HashMapIterator : public Iterator<V>::IteratorImpl {
    public:
        HashMapIterator(int bucket, std::function<V(int, int)> valueFunc, const HashMap& hashMap) :
            curBucket(bucket),
            curIndexInBucket(0),
            valueFunc(valueFunc),
            hashMap(hashMap) {};

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
        bool samePosition(typename const Iterator<V>::IteratorImpl& o) const override {
            const HashMapIterator& other = dynamic_cast<const HashMapIterator&>(o);
            return curBucket == other.curBucket && curIndexInBucket == other.curIndexInBucket;
        }

    private:
        const HashMap& hashMap;
        int curBucket, curIndexInBucket;

        std::function<V(int,int)> valueFunc;
    };


    int numEntries;
    ArrayList<ArrayList<KeyValuePair<T, U>>> buckets;
    std::function<int(const T)> hashFunction;
    double loadFactor;

    int hashKey(T key) {
        return ((hashFunction(key) % buckets.getSize()) + buckets.getSize()) % buckets.getSize();
    }

    int keyPositionInBucket(T key, int bucketIndex) {
        for(int i = 0; i < buckets.get(bucketIndex).getSize(); i++) {
            if(buckets.get(bucketIndex).get(i).key == key) {
                return i;
            }
        }
        return -1;
    }

    void expandAndRehash() {
        HashMap old = *this;

        buckets = ArrayList<ArrayList<KeyValuePair<T, U>>>();
        for(int i = 0; i < old.buckets.getSize() * 2; i++) {
            buckets.add(ArrayList<KeyValuePair<T, U>>());
        }

        for(const KeyValuePair<T, U> curPair : old) {
            put(curPair.key, curPair.value);
        }
    }
};