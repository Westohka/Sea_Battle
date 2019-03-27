#include "mainwindow.h"

#include <QApplication>

using namespace sea_battle;

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));
    QApplication a(argc, argv);
    QWidget *widget = new QWidget;
    MainWindow Window(widget);
    Window.show();

    return a.exec();
}
