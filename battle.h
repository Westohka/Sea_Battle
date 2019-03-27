#include "player.h"

#include <ostream>

namespace sea_battle {
    class battle {
    private:
        int _stage;
    public:
        player _player, _enemy;
        int _shipSelected, _shipDirection;

        battle();

        int row(double y);
        int col(double x);

        int stage();
        int stage(int i);

        ~battle();
    };
}
