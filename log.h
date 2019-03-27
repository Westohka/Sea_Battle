#include <QString>

namespace sea_battle {
    class log {
    private:
        QString _logs[5];
        int _length;

        void sort();
    public:
        log();

        int length();
        void push(const QString note);
        QString operator[](int i) const;

        ~log();
    };
}

