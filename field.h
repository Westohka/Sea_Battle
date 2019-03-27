#include <ostream>

namespace sea_battle {
    class field {
    private:
        int *_field;
    public:
        field();
        field(const field &f);

        int maxDist(int x, int y) const;
        int vDist(int x, int y) const;
        int hDist(int x, int y) const;

        field operator = (const field &A);
        int *operator[] (int index);
        const int *operator[] (int index) const;

        ~field();
    };
}

