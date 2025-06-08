// ItemNotFoundException.h
#pragma once
#include <stdexcept>
#include <string>

using namespace std;

class ItemNotFoundException : public runtime_error {
public:
    explicit ItemNotFoundException(const string& msg)
        : runtime_error(msg)
    {}
};
