#pragma once

#include "Collection.h"

template <class T>
class Iterator {
public:
    class IteratorImpl {
    public:
        IteratorImpl() {};

        IteratorImpl(const IteratorImpl& other) = delete;
        IteratorImpl& operator=(const IteratorImpl& other) noexcept = delete;
        IteratorImpl(IteratorImpl&& other) = delete;
        IteratorImpl& operator=(IteratorImpl&& other) noexcept = delete;

        virtual ~IteratorImpl() {};

        virtual T get() = 0;
        virtual void next() = 0;
        virtual bool samePosition(const IteratorImpl& other) const = 0;
    };

    Iterator(const Collection& collection, IteratorImpl* impl) :
        collection(collection),
        syncKey(collection.getSyncKey()),
        impl(impl) {
    };

    Iterator(const Iterator& other) = delete;
    Iterator& operator=(const Iterator& other) noexcept = delete;
    Iterator(Iterator&& other) = default; // Allow moves only
    Iterator& operator=(Iterator&& other) noexcept = delete;

    ~Iterator() {
        delete impl;
    }

    T operator*() {
        if(collection.checkSyncKey(syncKey)) {
            return impl->get();
        } else {
            throw ConcurrentModificationException("Detected modification of underlying Collection during Iterator get!");
        }
    };
    void operator++() {
        if(collection.checkSyncKey(syncKey)) {
            impl->next();
        } else {
            throw ConcurrentModificationException("Detected modification of underlying Collection during Iterator next!");
        }
    };

    bool operator!=(const Iterator& other) {
        if(&collection == &other.collection) {
            return !impl->samePosition(*other.impl);
        } else {
            throw UnrelatedIteratorException("The given Iterator does not iterate on the same Collection!");
        }
    }
private:

    const Collection& collection;
    const unsigned int syncKey;

    IteratorImpl* impl;
};
