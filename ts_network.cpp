#include "ts_network.h"
#include <iostream>
#include <QDebug>
#include <QThread>

TS_Network::TS_Network(QObject* parent) : QObject(parent)
{
    this->serv = new QTcpServer;

    /*QThread *thread = new QThread;
    this->moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(slot_start()));
    connect(this, SIGNAL(signal_closed()), thread, SLOT(quit()));
    connect(this, SIGNAL(signal_closed()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();*/

    connect(this->serv, SIGNAL(newConnection()), this, SLOT(newuser()));

    this->sys = new TS_System;
    this->sys->slot_createIndex();
}

void TS_Network::slot_start()
{
    if (!this->serv->listen(QHostAddress::Any, 8080))
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
    qDebug() << QString::fromUtf8("У нас новое соединение!");
    QTcpSocket* clientSocket=serv->nextPendingConnection();
    int idusersocs=clientSocket->socketDescriptor();
    SClients[idusersocs]=clientSocket;
    connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
}

void TS_Network::deleteuser()
{
    // Получаем объект сокета, который вызвал данный слот
    QTcpSocket* clientSocket = (QTcpSocket*)sender();

    // Получаем дескриптор, для того, чтоб в случае закрытия сокета удалить его из карты
    int idusersocs=clientSocket->socketDescriptor();

    clientSocket->close();
    SClients.remove(idusersocs);
    clientSocket->deleteLater();
}

void TS_Network::slotReadClient()
{
    // Получаем объект сокета, который вызвал данный слот
    QTcpSocket* clientSocket = (QTcpSocket*)sender();

    // Получаем дескриптор, для того, чтоб в случае закрытия сокета удалить его из карты
    int idusersocs=clientSocket->socketDescriptor();

    // Пример отправки ответа клиенту
    QTextStream os(clientSocket);

    os.setAutoDetectUnicode(true);

    os << generateHTML(QString::fromStdString(this->sys->getData().toStdString()));

    // Полученные данные от клиента выведем в qDebug,
    // можно разобрать данные например от GET запроса и по условию выдавать необходимый ответ.

    qDebug() << (clientSocket->readAll()+"\n\r");
}

QString TS_Network::generateHTML(QString buf)
{
    // формируем тело ответа (HTML)
    QString response_body = QString("<html><head><title>Test C++ HTTP Server</title></head>")
            + "<body>"
            + buf
            + "</body></html>";

    // Формируем весь ответ вместе с заголовками
    QString response = QString("HTTP/1.1 200 OK\r\n")
            + QString("Version: HTTP/1.1\r\n")
            + QString("Content-Type: text/html; charset=utf-8\r\n")
            + QString("Content-Length: ") + QString::number(response_body.length())
            + QString("\r\n\r\n")
            + response_body;

    return response;
}
