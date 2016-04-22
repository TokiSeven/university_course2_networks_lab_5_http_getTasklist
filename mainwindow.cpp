#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->net = new TS_Network(this);
    this->net->slot_start();

    slot_update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_update()
{
    QString str;
    for (int i = 0; i < this->net->getSystem()->getAllProcessList().size(); i++)
    {
        str = "PID: "
                + QString::number(this->net->getSystem()->getAllProcessList()[i].PID)
                + " NAME: "
                + this->net->getSystem()->getAllProcessList()[i].name
                + " MEMORY:"
                + QString::number(this->net->getSystem()->getAllProcessList()[i].memory);
        this->ui->listWidget->addItem(str);
    }
}
