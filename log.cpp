#include "log.h"

#include <cstring>
#include <iostream>

namespace sea_battle {
    log::log() {
        _length = 0;
    }

    int log::length() {
        return _length;
    }

    void log::push(QString note) {
        if (length() < 5) _length++;
        sort();
        _logs[0] = note;
    }

    void log::sort() {
        for (int i = length() - 1; i > 0; i--)
            _logs[i] = _logs[i - 1];
    }

    QString log::operator[](int i) const {
        return _logs[i];
    }

    log::~log() {}
}
