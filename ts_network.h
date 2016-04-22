#ifndef TS_NETWORK_H
#define TS_NETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include "ts_system.h"
#include <QMap>

class TS_Network : public QObject
{
    Q_OBJECT
public:
    TS_Network(QObject* parent = 0);

    //if server listening - true
    inline bool getStatus(){return this->status;}

    //return TS_Sytem
    inline TS_System* getSystem()const{return this->sys;}

public slots:
    //start listen (server)
    void slot_start();

private slots:
    //when message has coming
    void slotReadClient();

    //when opened new connection
    void newuser();

    //when client disconnect or server must do it
    void deleteuser();

private:
    //generate one QString from QList of processes (from TS_System)
    QString generateStringFromList();

    //DB of clients (connected)
    QMap<int, QTcpSocket*> SClients;

    //status of the server
    bool status;

    //generate full html page with headers
    QString generateHTML(QString);

    //QTcpServer for listening clients
    QTcpServer *serv;

    //in TS_System will be processes
    TS_System *sys;
};

#endif // TS_NETWORK_H
