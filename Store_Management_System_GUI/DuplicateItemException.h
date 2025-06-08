// DuplicateItemException.h
#pragma once
#include <stdexcept>
#include <string>

using namespace std;
class DuplicateItemException : public runtime_error {
public:
    explicit DuplicateItemException(const string& msg)
        : runtime_error(msg)
    {}
};
