#pragma once

#include <memory>

template <class T>
class Iterator {
public:
    class IteratorImpl {
    public:
        const void* collection;

        inline IteratorImpl(const void* collection) : collection(collection) {};

        IteratorImpl(const IteratorImpl& other) = delete;
        IteratorImpl& operator=(const IteratorImpl& other) noexcept = delete;
        IteratorImpl(IteratorImpl&& other) = delete;
        IteratorImpl& operator=(IteratorImpl&& other) noexcept = delete;

        inline virtual ~IteratorImpl() {};

        virtual T get() = 0;
        virtual void next() = 0;
        virtual bool atEnd() = 0;
    };

    inline Iterator(IteratorImpl* impl) : impl(impl) {};

    Iterator(const Iterator& other) = delete;
    Iterator& operator=(const Iterator& other) noexcept = delete;
    Iterator(Iterator&& other) = default; // Allow moves only
    Iterator& operator=(Iterator&& other) noexcept = delete;

    inline ~Iterator() {
        delete impl;
    }

    inline T operator*() { return impl->get(); };
    inline void operator++() { impl->next(); };

    inline bool operator!=(const Iterator& other) {
        if(impl->collection != other.impl->collection) {
            throw UnrelatedIteratorException("The given Iterator does not iterate on the same Collection!");
        }

        if(!impl->atEnd() && !other.impl->atEnd()) {
            return impl->get() != other.impl->get();
        } else {
            return !(impl->atEnd() && other.impl->atEnd());
        }
    }
private:
    IteratorImpl* impl;
};