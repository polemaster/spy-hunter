#include "sequence.h"
#include <stdio.h>

Sequence::Sequence() {
    _seq = NULL;
}

Sequence::~Sequence() {
    delete[] _seq;
}

void Sequence::createSequence(const int n) {
    if (_seq != NULL)
        delete[] _seq;
    _size = n;
    _seq = new bool[_size];
    for (int i = 0; i < _size; ++i) {
        _seq[i] = false;
    }
}

void Sequence::add(const int x) {
    if (x < 0 || x >= _size) {
        printf("Tried to add wrong sequence number: %d\n", x);
        return;
    }
    _seq[x] = true;
}

void Sequence::remove(const int x) {
    if (x < 0 || x >= _size) {
        printf("Tried to remove wrong sequence number: %d\n", x);
        return;
    }
    _seq[x] = false;
}

int Sequence::getRandom() {
    int sum = 0;
    for (int i = 0; i < _size; ++i) {
        if (!_seq[i])
            ++sum;
    }
    if (sum == 0) {
        return rand() % _size;
    }
    int x = rand() % sum;
    int tmp = 0;
    for (int i = 0; i < _size; ++i) {
        if ( tmp == x && (!_seq[i]) ) {
            _seq[i] = true;
            return i;
        }
        else if (!_seq[i]) {
            ++tmp;
        }
    }
    printf("Reached the end\n");
    return 0;
}
