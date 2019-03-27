#include "player.h"

using namespace std;

namespace sea_battle {
    player::player() {
        _x = -1; _y = -1;
        for (int i = 0; i < 4; i++)
            _ships[i] = 0;
    }
    player::player(bool random) : player() {
        if (random == true) {
            for (int i = 4; i > 0; i--) {
                while (_ships[i - 1] < 5 - i)
                    addShip(rand() % 10, rand() % 10, i, ((rand() % 2) * 90));
            }
        }
    }
    player::player(const player &p) {
        _selfField = p._selfField;
        _enemyField = p._enemyField;
        for (int i = 0; i < 4; i++)
            _ships[i] = p._ships[i];
    }

    field player::selfField() {
        return _selfField;
    }
    field player::enemyField() {
        return _enemyField;
    }

    // Добавление корабля на поле
    void player::addShip(int x, int y, int size, int direction) {
        if (x > 9 || x < 0 || y > 9 || y < 0 || size > 4 || size < 1 || (direction != 0 && direction != 90 && direction != 180 && direction != 270)) return;
        if (_ships[size - 1] >= 5 - size) return;

        if (placeFree(x, y, size, direction)) {
            for (int i = 0; i < size; i++) {
                if (direction == 0) _selfField[y][x + i] = size;
                if (direction == 90) _selfField[y + i][x] = size;
                if (direction == 180) _selfField[y][x - i] = size;
                if (direction == 270) _selfField[y - i][x] = size;
            }
            _ships[size - 1]++;
        }
    }
    // Удаление корабля с поля
    void player::deleteShip(int x, int y) {
        if (_selfField[y][x] == 0) return;
        _selfField[y][x] = 0;
        if (x < 9 && _selfField[y][x + 1] != 0) deleteShip(x + 1, y);
        if (x > 0 && _selfField[y][x - 1] != 0) deleteShip(x - 1, y);
        if (y < 9 && _selfField[y + 1][x] != 0) deleteShip(x, y + 1);
        if (y > 0 && _selfField[y - 1][x] != 0) deleteShip(x, y - 1);
    }
    // Проверка места под корабль
    bool player::placeFree(int x, int y, int size, int direction) {
        if (x > 9 || x < 0 || y > 9 || y < 0 || size > 4 || size < 1 || (direction != 0 && direction != 90 && direction != 180 && direction != 270)) return false;
        if ((direction == 0 && (x + size) > 10) || (direction == 180 && (x - size) < -1)  || (direction == 90 && (y + size) > 10) || (direction == 270 && (y - size) < -1)) return false;

        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= size; i++) {
                if (direction == 0) {
                    if ((i == -1 && x == 0) || (i == size && (x + size) == 10) || (j == -1 && y == 0) || (j == 1 && y == 9)) continue;
                    if (_selfField[y + j][x + i] != 0) return false;
                }
                if (direction == 90) {
                    if ((i == -1 && y == 0) || (i == size && (y + size) == 10) || (j == -1 && x == 0) || (j == 1 && x == 9)) continue;
                    if (_selfField[y + i][x + j] != 0) return false;
                }
                if (direction == 180) {
                    if ((i == -1 && x == 9) || (i == size && (x - size) == -1) || (j == -1 && y == 0) || (j == 1 && y == 9)) continue;
                    if (_selfField[y + j][x - i] != 0) return false;
                }
                if (direction == 270) {
                    if ((i == -1 && y == 9) || (i == size && (y - size) == -1) || (j == -1 && x == 0) || (j == 1 && x == 9)) continue;
                    if (_selfField[y - i][x + j] != 0) return false;
                }
            }
        }

        return true;
    }

    // Случайный выстрел
    bool player::shoot(player &p) {
        if (p.minShip() == 0) return false;

        // Добивание корабля
        if (_x != -1 && _enemyField[_y][_x] < 0 && _enemyField[_y][_x] != -5) {
            // Проверка, может ли корабль быть выше
            if (_y != 0 && (_enemyField[_y - 1][_x] == _enemyField[_y][_x] || _enemyField[_y + 1][_x] == _enemyField[_y][_x])) {
                for (int i = 1; _y - i >= 0; i++) {
                    if (_enemyField[_y - i][_x] == -5)
                        break;
                    if (_enemyField[_y - i][_x] == 0)
                        return shooted(_x, _y - i, p);
                }
            }

            // Проверка, может ли корабль быть ниже
            if (_y != 9 && (_enemyField[_y + 1][_x] == _enemyField[_y][_x] || _enemyField[_y - 1][_x] == _enemyField[_y][_x])) {
                for (int i = 1; _y + i < 10; i++) {
                    if (_enemyField[_y + i][_x] == -5)
                        break;
                    if (_enemyField[_y + i][_x] == 0)
                        return shooted(_x, _y + i, p);
                }
            }

            // Проверка, может ли корабль быть слева
            if (_x != 0 && (_enemyField[_y][_x - 1] == _enemyField[_y][_x] || _enemyField[_y][_x + 1] == _enemyField[_y][_x])) {
                for (int i = 1; _x - i >= 0; i++) {
                    if (_enemyField[_y][_x - i] == -5)
                        break;
                    if (_enemyField[_y][_x - i] == 0)
                        return shooted(_x - i, _y, p);
                }
            }

            // Проверка, может ли корабль быть справа
            if (_x != 9 && (_enemyField[_y][_x + 1] == _enemyField[_y][_x] || _enemyField[_y][_x - 1] == _enemyField[_y][_x])) {
                for (int i = 1; _x + i < 10; i++) {
                    if (_enemyField[_y][_x + i] == -5)
                        break;
                    if (_enemyField[_y][_x + i] == 0)
                        return shooted(_x + i, _y, p);
                }
            }

            // Выбор случайной точки подбитиия вокруг корабля
            int dir[4], d = rand() % 4;

            for (int i = 0; i < 4; i++)
                dir[i] = 1;

            if (_y == 0 || _enemyField[_y - 1][_x] == -5) dir[0] = 0;
            if (_x == 9 || _enemyField[_y][_x + 1] == -5) dir[1] = 0;
            if (_y == 9 || _enemyField[_y + 1][_x] == -5) dir[2] = 0;
            if (_x == 0 || _enemyField[_y][_x - 1] == -5) dir[3] = 0;

            if (p.minShip() > _enemyField.hDist(_x, _y)) {
                dir[1] = 0;
                dir[3] = 0;
            }
            if (p.minShip() > _enemyField.vDist(_x, _y)) {
                dir[0] = 0;
                dir[2] = 0;
            }

            while (dir[d] == 0) {
                if (d < 3) d++;
                    else d = 0;
            }

            if (d == 0) return shooted(_x, _y - 1, p);
            if (d == 1) return shooted(_x + 1, _y, p);
            if (d == 2) return shooted(_x, _y + 1, p);
            if (d == 3) return shooted(_x - 1, _y, p);
        }
        int step = p.maxShip();
        if (step == 3) step = 2;

        // Выбор случайной точки выстрела
        if (_x == -1) {
            _x = step - 1;
            _y = 0;

            if (step > 1) {
                int max = rand() % 24;
                for (int i = 0; i < max; i++) {
                    if (_x == 9) {
                        _x = 0;
                        _y++;
                    } else {
                        _x += step;
                        if (_x > 9) {
                            _x -= 9;
                            _y++;
                        }
                    }
                }
            }
        }

        while (_enemyField[_y][_x] != 0 || _enemyField.maxDist(_x, _y) < p.maxShip() || _x > 9 || _y > 9) {
            if (step > 1) {
                if (_y > 9) {
                    _x = step - 1;
                    _y = 0;
                } else {
                    if (_x == 9) {
                        _x = 0;
                        _y++;
                    } else {
                        _x += step;
                        if (_x > 9) {
                            _x -= 9;
                            _y++;
                        }
                    }
                }
            } else {
                _x = rand() % 10;
                _y = rand() % 10;
            }
        }

        return shooted(_x, _y, p);
    }

    // Обработка выстрела
    bool player::shooted(int x, int y, player &p) {
        if (_enemyField[y][x] != 0) return false;
        if (p._selfField[y][x] == 0) {
            p._selfField[y][x] = -5;
            _enemyField[y][x] = -5;
            if (_enemyField[_y][_x] == 0 || _enemyField[_y][_x] == -5) _x = -1;

            _log.push("Вы: " + (QString)('A' + x) + QString::number(y + 1) + " - мимо");
            p._log.push("Враг: " + (QString)('A' + x) + QString::number(y + 1) + " - мимо");
            return false;
        } else {
            p._selfField[y][x] *= -1;
            _enemyField[y][x] = p._selfField[y][x];
            bool destroyed = false;
            if (_enemyField[y][x] == -1) destroyed = true;
            else {
                int size = 1;
                for (int i = 1; (x - i) > -1; i++) {
                    if (_enemyField[y][x - i] == _enemyField[y][x]) size++;
                        else break;
                }
                for (int i = 1; (x + i) < 10; i++) {
                    if (_enemyField[y][x + i] == _enemyField[y][x]) size++;
                        else break;
                }
                for (int i = 1; (y - i) > -1; i++) {
                    if (_enemyField[y - i][x] == _enemyField[y][x]) size++;
                        else break;
                }
                for (int i = 1; (y + i) < 10; i++) {
                    if (_enemyField[y + i][x] == _enemyField[y][x]) size++;
                        else break;
                }

                if (size == -_enemyField[y][x]) destroyed = true;
            }
            if (destroyed) {
                _log.push("Вы: " + (QString)('A' + x) + QString::number(y + 1) + " - уничтожен");
                p._log.push("Враг: " + (QString)('A' + x) + QString::number(y + 1) + " - уничтожен");

                p._ships[-_enemyField[y][x] - 1]--;
                shipDestroy(x, y);
                _x = -1;
            } else {
                _log.push("Вы: " + (QString)('A' + x) + QString::number(y + 1) + " - подбит");
                p._log.push("Враг: " + (QString)('A' + x) + QString::number(y + 1) + " - подбит");
            }
            return true;
        }
    }

    // Уничтожение корабля
    void player::shipDestroy(int x, int y) {
        if (_enemyField[y][x] == -1 || _enemyField[y][x - 1] == _enemyField[y][x] || _enemyField[y][x + 1] == _enemyField[y][x]) {
            for (int i = 0; (x - i) > -1; i++) {
                if (_enemyField[y][x - i] == _enemyField[y][x]) {
                    if (y != 0) _enemyField[y - 1][x - i] = -5;
                    if (y != 9) _enemyField[y + 1][x - i] = -5;
                } else {
                    if (y != 0) _enemyField[y - 1][x - i] = -5;
                    if (y != 9) _enemyField[y + 1][x - i] = -5;
                    _enemyField[y][x - i] = -5;
                    break;
                }

            }
            for (int i = 1; (x + i) < 10; i++) {
                if (_enemyField[y][x + i] == _enemyField[y][x]) {
                    if (y != 0) _enemyField[y - 1][x + i] = -5;
                    if (y != 9) _enemyField[y + 1][x + i] = -5;
                } else {
                    if (y != 0) _enemyField[y - 1][x + i] = -5;
                    if (y != 9) _enemyField[y + 1][x + i] = -5;
                    _enemyField[y][x + i] = -5;
                    break;
                }
            }
        } else {
            for (int i = 0; (y - i) > -1; i++) {
                if (_enemyField[y - i][x] == _enemyField[y][x]) {
                    if (x != 0) _enemyField[y - i][x - 1] = -5;
                    if (x != 9) _enemyField[y - i][x + 1] = -5;
                } else {
                    if (x != 0) _enemyField[y - i][x - 1] = -5;
                    if (x != 9) _enemyField[y - i][x + 1] = -5;
                    _enemyField[y - i][x] = -5;
                    break;
                }
            }
            for (int i = 1; (y + i) < 10; i++) {
                if (_enemyField[y + i][x] == _enemyField[y][x]) {
                    if (x != 0) _enemyField[y + i][x - 1] = -5;
                    if (x != 9) _enemyField[y + i][x + 1] = -5;
                } else {
                    if (x != 0) _enemyField[y + i][x - 1] = -5;
                    if (x != 9) _enemyField[y + i][x + 1] = -5;
                    _enemyField[y + i][x] = -5;
                    break;
                }
            }
        }
    }

    int player::minShip() {
        for (int i = 0; i < 4; i++)
            if (_ships[i] > 0) return (i + 1);
        return 0;
    }
    int player::maxShip() {
        for (int i = 3; i >= 0; i--)
            if (_ships[i] > 0) return (i + 1);
        return 0;
    }

    player::~player() {}

}
