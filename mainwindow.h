#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QRect>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QTimer>

#include "battle.h"

namespace Ui {
    class MainWindow;
}

namespace sea_battle {
    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        Ui::MainWindow *ui;

        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_bQuit_pressed();
        void on_bNewGame_pressed();
        void on_bReady_pressed();
        void on_bRestart_pressed();
        void on_bSurrender_pressed();
        void on_bShipRandom_pressed();

        void updateOpacity();
        void updateSize();
        void updatePosY();
        void updateShootAi();

    private:
        battle _battle;
        unsigned int _GameStatus, _mouseX, _mouseY;
        double _opacity, _size, _posY;

        QTimer *_timer, *_timer1;

        void paintEvent(QPaintEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        bool event(QEvent *event);

        bool pointInRectangle(double rx, double ry, double x1, double y1, double x2, double y2);
    };
}
