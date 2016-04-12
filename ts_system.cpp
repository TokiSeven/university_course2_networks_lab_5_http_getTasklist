#include "ts_system.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <QFile>
#include <QDebug>

using namespace std;

TS_System::TS_System(QString cmd, QObject *parent)
    : QObject(parent)
{
    this->command = cmd;
    this->filename = "Buffer.txt";

    getInfoIntoFile();
    readData();
}

void TS_System::readData()
{
    // создаем объект класса QFile
    QFile file(this->filename);
    // Проверяем, возможно ли открыть наш файл для чтения
    if (!file.open(QIODevice::ReadOnly))
        // если это сделать невозможно, то завершаем функцию
        return;
    //считываем все данные с файла в объект data
    this->data = file.readAll();
}

void TS_System::getInfoIntoFile()
{
    freopen(this->filename.toStdString().c_str(), "w", stdout);
    system("tasklist");
    freopen("CONOUT$", "w", stdout);
}
