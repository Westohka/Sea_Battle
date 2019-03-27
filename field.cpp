#include "field.h"

namespace sea_battle {
    field::field() {
        _field = new int [100];
        for (int i = 0; i < 100; i++)
            _field[i] = 0;
    }
    field::field(const field &f) {
        _field = new int [100];
        for (int i = 0; i < 100; i++)
            _field[i] = f._field[i];
    }

    int field::maxDist(int x, int y) const {
        if (x > 9 || y > 9) return 0;
        int hmax = hDist(x, y);
        int vmax = vDist(x, y);
        return hmax > vmax ? hmax : vmax;
    }

    int field::hDist(int x, int y) const {
        if (x > 9 || y > 9) return 0;
        int hmax = 0;
        for (int i = 1; i < 10 - x; i++) {
            if (_field[(y * 10) + x + i] != 0) break;
            hmax++;
        }
        for (int i = 1; i <= x; i++) {
            if (_field[(y * 10) + x - i] != 0) break;
            hmax++;
        }
        return hmax + 1;
    }
    int field::vDist(int x, int y) const {
        if (x > 9 || y > 9) return 0;
        int vmax = 0;
        for (int i = 1; i < 10 - y; i++) {
            if (_field[((y + i) * 10) + x] != 0) break;
            vmax++;
        }
        for (int i = 1; i <= y; i++) {
            if (_field[((y - i) * 10) + x] != 0) break;
            vmax++;
        }
        return vmax + 1;
    }

    field field::operator = (const field &A) {
        for (int i = 0; i < 100; i++)
            _field[i] = A._field[i];
        return (*this);
    }
    int *field::operator[] (int index) {
        return _field + index * 10;
    }
    const int *field::operator[] (int index) const {
        return _field + index * 10;
    }

    field:: ~field() {
        if (_field) delete[] _field;
    }
}

