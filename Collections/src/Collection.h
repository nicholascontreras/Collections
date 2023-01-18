#pragma once

#include <functional>

#include "errors.h"

class Collection {
public:
    virtual int getSize() const = 0;

    unsigned int getSyncKey() const { return syncKey; };
    void rotateSyncKey() { syncKey++; };
    bool checkSyncKey(unsigned int key) const { return key == syncKey; };

protected:
    Collection() : syncKey(0) {};

private:
    unsigned int syncKey;
};

template <class>
class Iterator;

template <class T>
class TemplatedCollection : public Collection {
public:

    virtual Iterator<T&> begin() = 0;
    virtual Iterator<T&> end() = 0;

    virtual Iterator<const T&> begin() const = 0;
    virtual Iterator<const T&> end() const = 0;
};