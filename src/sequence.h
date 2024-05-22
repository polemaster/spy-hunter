#pragma once
#include "constants.h"

class Sequence {
public:
    Sequence();
    ~Sequence();
    int getRandom();
    void remove(const int index);
    void createSequence(const int n);
    void add(const int x);
private:
    bool *_seq;
    int _size;
};
