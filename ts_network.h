#ifndef TS_NETWORK_H
#define TS_NETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include "ts_system.h"
#include <QMap>

struct CLIENT
{
    QTcpSocket *soc;
    quint16 size = 0;
};

class TS_Network : public QObject
{
    Q_OBJECT
public:
    TS_Network(QObject* parent = 0);

    inline bool getWorking(){return this->isWorking;}
    inline TS_System* getSystem()const{return this->sys;}

public slots:
    void slot_start();

signals:
    void signal_closed();

private slots:
    void slotReadClient();
    void newuser();
    void deleteuser();

private:
    QMap<int, QTcpSocket*> SClients;
    bool isWorking;
    QString generateHTML(QString);
    QTcpServer *serv;
    TS_System *sys;
};

#endif // TS_NETWORK_H
