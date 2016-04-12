#include "ts_network.h"
#include <iostream>
#include <QDebug>

TS_Network::TS_Network(QObject* parent) : QObject(parent)
{
    this->serv = new QTcpServer;

    connect(this->serv, SIGNAL(newConnection()), this, SLOT(newuser()));

    if (!this->serv->listen(QHostAddress::Any, 80))
    {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(this->serv->errorString());
    }
    else
    {
        qDebug() << QString::fromUtf8("Server was started!");
    }
}


void TS_Network::newuser()
{
    qDebug() << "new connection!";

    QTcpSocket* clientSocket = serv->nextPendingConnection();

    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(deleteuser()));
}

void TS_Network::slotReadClient()
{
    // Получаем объект сокета, который вызвал данный слот
    QTcpSocket* clientSocket = (QTcpSocket*)sender();

    //тут обрабатываются данные от сервера
    QDataStream in(clientSocket);

    //читаем, что прислал
    QString mess;
    in >> mess;

    //------------->>>>>>>>>>>отправляем ответ - страницу
    TS_System cmd;

    this->send_to_one(clientSocket, generateHTML(QString::fromStdString(cmd.getData().toStdString())));
}

void TS_Network::send_to_one(QTcpSocket *socket, QString data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << data;

    socket->write(block);
}

QString TS_Network::generateHTML(QString buf)
{
    // формируем тело ответа (HTML)
    QString response_body = QString("<title>Test C++ HTTP Server</title>\n") + buf;

    // Формируем весь ответ вместе с заголовками
    QString response = QString("HTTP/1.1 200 OK\r\n")
            + QString("Version: HTTP/1.1\r\n")
            + QString("Content-Type: text/html; charset=utf-8\r\n")
            + QString("Content-Length: ") + QString::number(response_body.length())
            + QString("\r\n\r\n")
            + response_body;

    return response;
}
