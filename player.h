#include "field.h"
#include "log.h"

namespace sea_battle {
    class player {
    private:
        field _selfField, _enemyField;
        int _x, _y;

        void shipDestroy(int x, int y);
    public:
        int _ships[4];
        log _log;

        player();
        player(bool random);
        player(const player &p);

        field selfField();
        field enemyField();

        void addShip(int x, int y, int size, int direction);
        void deleteShip(int x, int y);
        bool placeFree(int x, int y, int size, int direction);
        bool shoot(player &p);
        bool shooted(int x, int y, player &p);
        int minShip();
        int maxShip();

        ~player();
    };
}

