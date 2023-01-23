#pragma once

#include <stdexcept>
#include <string>

#define DEFINE_EXCEPTION(NAME) \
class NAME : public std::exception { \
public: \
    inline NAME(std::string message) : message(message) {}; \
    inline const char* what() const override { return message.c_str(); } \
private: \
    std::string message; \
};

DEFINE_EXCEPTION(IndexOutOfBoundsException)
DEFINE_EXCEPTION(NoSuchItemException)
DEFINE_EXCEPTION(ConcurrentModificationException)
DEFINE_EXCEPTION(UnrelatedIteratorException)

#undef DEFINE_EXCEPTION
