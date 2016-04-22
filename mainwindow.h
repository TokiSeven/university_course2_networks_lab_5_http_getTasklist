#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ts_network.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    //print processes from TS_System (inside TS_network) on the window
    void slot_update();

private:
    //TS_Network - QTcpServer
    TS_Network *net;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
