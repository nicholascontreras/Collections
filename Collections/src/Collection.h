#pragma once

#include "errors.h"

#include "Iterator.h"

template <class T>
class Collection {
public:
    virtual Iterator<T&> begin() = 0;
    virtual Iterator<T&> end() = 0;

    virtual Iterator<const T&> begin() const = 0;
    virtual Iterator<const T&> end() const = 0;

    virtual int getSize() const = 0;
private:
    unsigned int startingSyncKey;

protected:
    inline Collection() : startingSyncKey(0) {};

    inline unsigned int getSyncKey() const { return startingSyncKey; };
    inline void rotateSyncKey() { startingSyncKey++; };
    inline bool checkSyncKey(unsigned int key) const { return key == startingSyncKey; };
};