#ifndef TS_NETWORK_H
#define TS_NETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include "ts_system.h"

class TS_Network : public QObject
{
    Q_OBJECT
public:
    TS_Network(QObject* parent = 0);

private slots:
    void slotReadClient();
    void newuser();
    void send_to_one(QTcpSocket *socket, QString data);

private:
    QString generateHTML(QString);
    QTcpServer *serv;
};

#endif // TS_NETWORK_H
