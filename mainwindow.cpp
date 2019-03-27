#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <QCursor>

namespace sea_battle {
    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
        ui = new Ui::MainWindow;
        ui->setupUi(this);
        ui->bReady->hide();
        ui->bRestart->hide();
        ui->wBatte->hide();

        _battle = battle();
        _GameStatus = 0;
        _opacity = 0;
        _size = 1;
        _posY = -405;
        _mouseX = 0; _mouseY = 0;

        _timer = 0;

        _timer1 = new QTimer();
        _timer1->setInterval(1);
        connect(_timer1, SIGNAL(timeout()), this, SLOT(updatePosY()));
        _timer1->start();
    }

    void MainWindow::paintEvent(QPaintEvent *) {
        if (_GameStatus == 1) {
            QPainter painter(this);
            painter.setOpacity(_opacity);
            painter.setPen(QPen (Qt::darkGray));
            painter.setFont(QFont ("Times", 12, QFont::Bold));
            if (_battle.stage() != 4) {
                painter.drawText(QPoint(68, 50 + _posY), QString("Ваше поле:"));
                painter.drawText(QPoint(472, 50 + _posY), QString("Поле противника:"));
            } else {
                painter.drawText(QPoint(68, 50 + _posY), QString("Корабли противника:"));
                painter.drawText(QPoint(472, 50 + _posY), QString("Вы стреляли:"));
            }
            painter.setPen(QPen ());

            /// Поля
            painter.setFont(QFont ("Times", 12, QFont::Normal));
            for (int i = 0; i < 10; i++) {
                painter.drawText(QPoint(54 + (i * 22), 80 + _posY), QString("ABCDEFGHIJ").at(i));
                painter.drawText(QPoint(28, 106 + (i * 22) + _posY), QString::number(i + 1));

                painter.drawText(QPoint(458 + (i * 22), 80 + _posY), QString("ABCDEFGHIJ").at(i));
                painter.drawText(QPoint(432, 106 + (i * 22) + _posY), QString::number(i + 1));

                for (int j = 0; j < 10; j++) {
                    /// Левое поле
                    painter.drawRect(48 + (i * 22), 90 + (j * 22) + _posY, 22, 22);
                    if (_battle.stage() != 4) {
                        if (_battle._player.selfField()[j][i] != 0) {
                            if (_battle._player.selfField()[j][i] == -5)
                                painter.setBrush(QBrush (Qt::cyan));
                            else if (_battle._player.selfField()[j][i] > 0)
                                painter.setBrush(QBrush (Qt::black));
                            else painter.setBrush(QBrush (Qt::red));
                            painter.drawEllipse(54 + (i * 22), 96 + (j * 22) + _posY, 10, 10);
                        }
                    } else {
                        if (_battle._enemy.selfField()[j][i] != 0) {
                            if (_battle._enemy.selfField()[j][i] == -5)
                                painter.setBrush(QBrush (Qt::cyan));
                            else if (_battle._enemy.selfField()[j][i] > 0)
                                painter.setBrush(QBrush (Qt::black));
                            else painter.setBrush(QBrush (Qt::red));
                            painter.drawEllipse(54 + (i * 22), 96 + (j * 22) + _posY, 10, 10);
                        }
                    }
                    painter.setBrush(QBrush ());

                    /// Правое поле
                    painter.drawRect(452 + (i * 22), 90 + (j * 22) + _posY, 22, 22);
                    if (_battle._player.enemyField()[j][i] != 0) {
                        if (_battle._player.enemyField()[j][i] == -5)
                            painter.setBrush(QBrush (Qt::cyan));
                        else painter.setBrush(QBrush (Qt::red));
                        painter.drawEllipse(458 + (i * 22), 96 + (j * 22) + _posY, 10, 10);
                        painter.setBrush(QBrush ());
                    }
                }
            }

            /// Информация о стадии игры
            if (_battle.stage() == 0) {
                painter.setFont(QFont ("Times", 18));
                painter.drawText(0, 0, 720, 40, Qt::AlignCenter, QString("РАССТАВЬТЕ СВОИ КОРАБЛИ"));
            } else if (_posY == 20) {
                if (!_timer && _size < 18) {
                    _timer = new QTimer();
                    _timer->setInterval(1);
                    connect(_timer, SIGNAL(timeout()), this, SLOT(updateSize()));
                    _timer->start();
                }
                painter.setFont(QFont ("Times", _size, QFont::Bold));
                if (_battle.stage() < 3) painter.drawText(0, 0, 720, 405, Qt::AlignCenter, QString("БОЙ"));
            }

            painter.setFont(QFont ("Times", 16));
            if (_battle.stage() == 3) {
                painter.setPen(QPen (Qt::darkGreen));
                painter.drawText(0, 0, 720, 405, Qt::AlignCenter, QString("ВЫ ПОБЕДИЛИ"));
            }
            if (_battle.stage() == 4) {
                painter.setPen(QPen (Qt::darkRed));
                painter.drawText(0, 0, 720, 405, Qt::AlignCenter, QString("ВЫ ПРОИГРАЛИ"));
            }
            painter.setPen(QPen ());

            painter.setFont(QFont ("Times", 18));
            if (_battle.stage() == 1)
                painter.drawText(0, 0, 720, 40, Qt::AlignCenter, QString("ВАШ ХОД"));
            if (_battle.stage() == 2)
                painter.drawText(0, 0, 720, 40, Qt::AlignCenter, QString("ХОД ПРОТИВНИКА"));

            /// Расстановка кораблей
            if (_battle.stage() == 0) {
                painter.setFont(QFont ("Times", 12, QFont::Normal));
                painter.setBrush(QBrush (Qt::black));
                for (int i = 4; i > 2; i--) {
                    if (_battle._player._ships[i - 1] < (5 - i)) painter.setOpacity(_opacity);
                        else painter.setOpacity(0.2);
                    for (int j = 0; j < i; j++)
                        painter.drawEllipse(54 + (j * 22), 320 + ((5 - i) * 22), 10, 10);
                    painter.drawText(QPoint(140, 330 + ((5 - i) * 22)), "x" + QString::number(5 - i - _battle._player._ships[i - 1]));
                }
                for (int i = 2; i > 0; i--) {
                    if (_battle._player._ships[i - 1] < (5 - i)) painter.setOpacity(_opacity);
                        else painter.setOpacity(0.2);
                    for (int j = 0; j < i; j++)
                        painter.drawEllipse(200 + (j * 22), 320 + ((3 - i) * 22), 10, 10);
                    painter.drawText(QPoint(242, 330 + ((3 - i) * 22)), "x" + QString::number(5 - i - _battle._player._ships[i - 1]));
                }

                painter.setOpacity(_opacity);
                if (_battle._shipSelected != 0) {
                    if (!pointInRectangle(_mouseX, _mouseY, 48, 90, 268, 310))
                        painter.setBrush(QBrush (Qt::red));
                    else if (_battle._player.placeFree(_battle.col(_mouseX), _battle.row(_mouseY), _battle._shipSelected, _battle._shipDirection))
                        painter.setBrush(QBrush (Qt::green));
                    else painter.setBrush(QBrush (Qt::red));
                    for (int j = 0; j < _battle._shipSelected; j++) {
                        if (_battle._shipDirection == 0)
                            painter.drawEllipse(_mouseX + (j * 22), _mouseY, 10, 10);
                        if (_battle._shipDirection == 90)
                            painter.drawEllipse(_mouseX, _mouseY + (j * 22), 10, 10);
                        if (_battle._shipDirection == 180)
                            painter.drawEllipse(_mouseX - (j * 22), _mouseY, 10, 10);
                        if (_battle._shipDirection == 270)
                            painter.drawEllipse(_mouseX, _mouseY - (j * 22), 10, 10);
                    }
                }
                painter.setBrush(QBrush ());
            }

            /// Логи
            painter.setFont(QFont ("Times", 12));
            if (_battle._player._log.length() > 0) {
                for (int i = 0; i < _battle._player._log.length(); i++) {
                    painter.setOpacity(1 - ((double)i / _battle._player._log.length()));
                    painter.drawText(QPoint(300, 280 + (i * 20)), _battle._player._log[i]);
                }
            }
        }
    }

    bool MainWindow::event(QEvent *event) {
        /// Обработка нажатия клавиш клавиатуры
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *key = (QKeyEvent*)event;
            if (key->key() == Qt::Key_Escape) {
                if (_battle._shipSelected != 0) {
                    _battle._player._ships[_battle._shipSelected - 1]--;
                    _battle._shipSelected = 0;
                    repaint();
                }

                QMessageBox::StandardButton bQuit = QMessageBox::question(this, "Выход", "Вы действительно хотите выйти?", QMessageBox::Yes | QMessageBox::No);
                if (bQuit == QMessageBox::Yes) QApplication::quit();

                return true;
            }
            if (_battle._shipSelected != 0) {
                if (key->key() == Qt::Key_Q) {
                    if (_battle._shipDirection == 0) _battle._shipDirection = 270;
                        else _battle._shipDirection -= 90;
                    repaint(_mouseX - 88, _mouseY - 88, _mouseX + 88, _mouseY + 88);
                    return true;
                }
                if (key->key() == Qt::Key_E) {
                    if (_battle._shipDirection == 270) _battle._shipDirection = 0;
                        else _battle._shipDirection += 90;
                    repaint(_mouseX - 88, _mouseY - 88, _mouseX + 88, _mouseY + 88);
                    return true;
                }
            }
        }
        /// Обработка нажатия клавиш мыши
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouse = (QMouseEvent*)event;
            if (mouse->button() == Qt::LeftButton) {
                /// Расстановка кораблей
                if (_battle.stage() == 0) {
                    if (pointInRectangle(mouse->x(), mouse->y(), 50, 335, 138, 355) && _battle._player._ships[3] < 1) {
                        _battle._shipSelected = 4;
                        _battle._shipDirection = 0;
                        _battle._player._ships[3]++;
                        _mouseX = mouse->x();
                        _mouseY = mouse->y();
                        repaint();
                        return true;
                    }
                    if (pointInRectangle(mouse->x(), mouse->y(), 50, 360, 115, 380) && _battle._player._ships[2] < 2) {
                        _battle._shipSelected = 3;
                        _battle._shipDirection = 0;
                        _battle._player._ships[2]++;
                        _mouseX = mouse->x();
                        _mouseY = mouse->y();
                        repaint();
                        return true;
                    }
                    if (pointInRectangle(mouse->x(), mouse->y(), 195, 335, 235, 355) && _battle._player._ships[1] < 3) {
                        _battle._shipSelected = 2;
                        _battle._shipDirection = 0;
                        _battle._player._ships[1]++;
                        _mouseX = mouse->x();
                        _mouseY = mouse->y();
                        repaint();
                        return true;
                    }
                    if (pointInRectangle(mouse->x(), mouse->y(), 195, 360, 215, 380) && _battle._player._ships[0] < 4) {
                        _battle._shipSelected = 1;
                        _battle._shipDirection = 0;
                        _battle._player._ships[0]++;
                        _mouseX = mouse->x();
                        _mouseY = mouse->y();
                        repaint();
                        return true;
                    }
                    if (pointInRectangle(mouse->x(), mouse->y(), 48, 90, 268, 310) && _battle._player.selfField()[_battle.row(mouse->y())][_battle.col(mouse->x())] != 0) {
                        ui->bReady->hide();
                        _battle._player._ships[_battle._player.selfField()[_battle.row(mouse->y())][_battle.col(mouse->x())] - 1]--;
                        _battle._player.deleteShip(_battle.col(mouse->x()), _battle.row(mouse->y()));
                        repaint();
                        return true;

                    }
                }
                /// Ход игрока
                if (_battle.stage() == 1) {
                    if (pointInRectangle(mouse->x(), mouse->y(), 452, 90 + _posY, 672, 310 + _posY) && _battle._player.enemyField()[_battle.row(mouse->y() - _posY)][_battle.col(mouse->x() - 404)] == 0) {
                        if (!_battle._player.shooted(_battle.col(mouse->x() - 404), _battle.row(mouse->y() - _posY), _battle._enemy)) {
                            _battle.stage(2);
                            ui->bSurrender->hide();
                            QTimer::singleShot(300, this, SLOT(updateShootAi()));
                        }
                        if (_battle._enemy.minShip() == 0) {
                            _battle.stage(3);
                            ui->bRestart->show();
                            ui->bSurrender->hide();
                        }
                        repaint();
                        return true;
                    }
                }
            }
        }
        /// Обработка отпускания клавиш мыши
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouse = (QMouseEvent*)event;
            if (mouse->button() == Qt::LeftButton) {
                if (_battle._shipSelected != 0) {
                    _battle._player._ships[_battle._shipSelected - 1]--;
                    if (pointInRectangle(_mouseX, _mouseY, 48, 90, 268, 310))
                        _battle._player.addShip(_battle.col(_mouseX), _battle.row(_mouseY), _battle._shipSelected, _battle._shipDirection);
                    _battle._shipSelected = 0;
                    if (_battle._player._ships[0] == 4 && _battle._player._ships[1] == 3 && _battle._player._ships[2] == 2 && _battle._player._ships[3] == 1)
                        ui->bReady->show();
                    repaint();
                    return true;
                }
            }
        }

        return QWidget::event(event);
    }

    void MainWindow::mouseMoveEvent(QMouseEvent *event) {
        if (_battle._shipSelected != 0) {
            _mouseX = event->x();
            _mouseY = event->y();
            repaint();
        }
    }

    void MainWindow::updateOpacity() {
        _opacity += 0.01;
        if (_opacity >= 1) {
            _timer->stop();
            delete _timer;
            _timer = 0;
        }
        repaint();
    }
    void MainWindow::updateSize() {
        _size += 1;
        if (_size >= 18) {
            _timer->stop();
            delete _timer;
            _timer = 0;
        }
        repaint();
    }
    void MainWindow::updatePosY() {
        if ((_posY >= 20 && _GameStatus == 1) || (_GameStatus == 0 && _posY >= 0)) {
            _timer1->stop();
            delete _timer1;
            _timer1 = 0;
            return;
        }

        if (_GameStatus == 0) {
            _posY += 4;
            ui->wMainMenu->setGeometry(0, _posY, 721, 405);
        } else _posY += 1;

        repaint();
    }
    void MainWindow::updateShootAi() {
        if (_battle.stage() == 2) {
            if (!_battle._enemy.shoot(_battle._player)) {
                _battle.stage(1);
                ui->bSurrender->show();
            } else QTimer::singleShot(300, this, SLOT(updateShootAi()));
            if (_battle._player.minShip() == 0) {
                _battle.stage(4);
                ui->bSurrender->hide();
                ui->bRestart->show();
            }
            repaint();
        }
    }

    bool MainWindow::pointInRectangle(double rx, double ry, double x1, double y1, double x2, double y2) {
        return (rx >= x1 && rx <= x2 && ry >= y1 && ry <= y2);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::on_bReady_pressed() {
        _battle.stage(1);
        ui->bReady->hide();
        ui->bShipRandom->hide();

        _timer1 = new QTimer();
        _timer1->setInterval(1);
        connect(_timer1, SIGNAL(timeout()), this, SLOT(updatePosY()));
        _timer1->start();
    }

    void MainWindow::on_bQuit_pressed() {
        QMessageBox::StandardButton bQuit = QMessageBox::question(this, "Выход", "Вы действительно хотите выйти?", QMessageBox::Yes | QMessageBox::No);
        if (bQuit == QMessageBox::Yes) QApplication::quit();
    }

    void MainWindow::on_bNewGame_pressed() {
        _GameStatus = 1;
        ui->wMainMenu->hide();

        _battle = battle();
        _posY = 0;
        _opacity = 0;
        ui->wBatte->show();

        if (_timer) {
            _timer->stop();
            delete _timer;
            _timer = 0;
        }
        _timer = new QTimer();
        _timer->setInterval(1);
        connect(_timer, SIGNAL(timeout()), this, SLOT(updateOpacity()));
        _timer->start();
    }

    void MainWindow::on_bRestart_pressed() {
        _battle = battle();
        _posY = 0;

        ui->bRestart->hide();
        ui->bSurrender->show();
        ui->bShipRandom->show();
        repaint();
    }

    void MainWindow::on_bSurrender_pressed() {
        if (_battle.stage() < 2 && !_timer) {
            _battle.stage(4);
            ui->bSurrender->hide();
            ui->bRestart->show();
            ui->bShipRandom->hide();
            repaint();
        }
    }

    void MainWindow::on_bShipRandom_pressed() {
        ui->bShipRandom->hide();
        _battle._player = player(true);

        _battle.stage(1);
        ui->bReady->hide();
        _timer1 = new QTimer();
        _timer1->setInterval(1);
        connect(_timer1, SIGNAL(timeout()), this, SLOT(updatePosY()));
        _timer1->start();
    }
}
