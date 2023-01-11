#pragma once

#include <functional>

#include "errors.h"

//#include "Iterator.h"

template <class>
class Iterator;

template <class T>
class Collection {
public:

    virtual Iterator<T&> begin() = 0;
    virtual Iterator<T&> end() = 0;

    virtual Iterator<const T&> begin() const = 0;
    virtual Iterator<const T&> end() const = 0;

    virtual int getSize() const = 0;
private:
    unsigned int syncKey;

protected:
    Collection() : syncKey(0) {};

    unsigned int getSyncKey() const { return syncKey; };
    void rotateSyncKey() { syncKey++; };
    bool checkSyncKey(unsigned int key) const { return key == syncKey; };
};