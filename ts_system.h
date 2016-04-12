#ifndef TS_SYSTEM_H
#define TS_SYSTEM_H

#include <QObject>
#include <QString>
#include <QByteArray>

class TS_System : public QObject
{
    Q_OBJECT
public:
    explicit TS_System(QString cmd = "tasklist", QObject *parent = 0);

    QByteArray getData()const{return this->data;}

private:
    void readData();
    void getInfoIntoFile();

    QString filename;
    QString command;
    QByteArray data;
};

#endif // TS_PROCESS_TASKLIST_H