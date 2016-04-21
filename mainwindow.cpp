#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->net = new TS_Network(this);
    this->net->slot_start();

    QTimer::singleShot(3000, this, SLOT(slot_update()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_update()
{
    this->ui->listWidget->addItem(QString::fromStdString(this->net->getSystem()->getData().toStdString()));
}
