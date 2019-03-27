#include "battle.h"

namespace sea_battle {
    battle::battle() {
        _player = player();
        _enemy = player(true);
        _stage = 0;
        _shipSelected = 0;
        _shipDirection = 0;
    }

    int battle::row(double y) {
        if (y >= 288) return 9;
        else if (y >= 266) return 8;
        else if (y >= 244) return 7;
        else if (y >= 222) return 6;
        else if (y >= 200) return 5;
        else if (y >= 178) return 4;
        else if (y >= 156) return 3;
        else if (y >= 134) return 2;
        else if (y >= 112) return 1;
        else return 0;
    }

    int battle::col(double x) {
        if (x >= 246) return 9;
        else if (x >= 224) return 8;
        else if (x >= 202) return 7;
        else if (x >= 180) return 6;
        else if (x >= 158) return 5;
        else if (x >= 136) return 4;
        else if (x >= 114) return 3;
        else if (x >= 92) return 2;
        else if (x >= 70) return 1;
        else return 0;
    }

    int battle::stage() {
        return _stage;
    }
    int battle::stage(int i) {
        return _stage = i;
    }

    battle::~battle() {}

}
