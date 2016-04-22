#include "ts_network.h"
#include <iostream>
#include <QDebug>
#include <QThread>
#include <QProcess>

TS_Network::TS_Network(QObject* parent) : QObject(parent)
{
    qDebug() << "TS_Network >> Object created.";

    //Create QTcpServer
    this->serv = new QTcpServer;

    //when something create new connection, it will be in newuser*(;
    connect(this->serv, SIGNAL(newConnection()), this, SLOT(newuser()));

    //create TS_System
    this->sys = new TS_System;

    //we did not started server
    this->status = false;
}

void TS_Network::slot_start()
{
    qDebug() << "TS_Network >> slot_start();";

    if (!this->status)
    {
        if (!this->serv->listen(QHostAddress::Any, 8080))
        {
            qDebug() <<  QObject::tr("TS_Network >> slot_start(); >> Unable to start the server: %1.").arg(this->serv->errorString());
            this->status = false;
        }
        else
        {
            qDebug() << "TS_Network >> slot_start(); >> Server was started!";
            this->status = true;
        }
    }
}


void TS_Network::newuser()
{
    qDebug() << "TS_Network >> newuser();";

    if (this->status)
    {
        //clientSocket is a current connection
        QTcpSocket* clientSocket=serv->nextPendingConnection();

        //idusersocs - socket descriptor of current connection
        int idusersocs=clientSocket->socketDescriptor();

        //add in DB (SClients) new socket
        SClients[idusersocs]=clientSocket;

        //when this client something send, it will come to slotReadClient(); (async)
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
    }
}

void TS_Network::deleteuser()
{
    qDebug() << "TS_Network >> deleteuser();";

    if (this->status)
    {
        //modify sender() (QObject) to clientSocket (QTcpSocket)
        QTcpSocket* clientSocket = (QTcpSocket*)sender();

        //get their descriptor
        int idusersocs=clientSocket->socketDescriptor();

        //close connection
        clientSocket->close();

        //remove from DB
        SClients.remove(idusersocs);

        //delete clientSocket
        clientSocket->deleteLater();
    }
}

QString TS_Network::generateStringFromList()
{
    qDebug() << "TS_Network >> generateStringFromList();";

    //str - result of this function
    QString str;

    //create table in str
    str = "<table border = '1px' cellpadding = '5px'>";

    //first row is info about column
    str += "<tr><td>Number</td><td>PID</td><td>Name</td><td>Memory used (KByte)</td></tr>\n";

    //get all processes from TS_System in strs
    QList<TS_Process> strs = this->sys->getAllProcessList();

    //go throw all process
    for (int i = 0; i < strs.size(); i++)
    {
        //create one row from data (strs[i] - current process)
        str += "<tr><td>" + QString::number(i + 1) + "</td>";
        str += "<td>" + QString::number(strs[i].PID) + "</td>";
        str += "<td>" + strs[i].name + "</td>";
        str += QString("<td>") + QString::number(strs[i].memory) + "</td>";
        str += "</tr>\n";
    }

    //close table's tag
    str += "</table>";

    //return result
    return str;
}

void TS_Network::slotReadClient()
{
    qDebug() << "TS_Network >> slotReadClient();";

    if (this->status)
    {
        //modify sender() (QObject) to clientSocket (QTcpSocket)
        QTcpSocket* clientSocket = (QTcpSocket*)sender();

        //idusersocs - socket descriptor of current connection
        int idusersocs=clientSocket->socketDescriptor();

        //clientSocket->readAll() will return QByteArray of all client's message
        qDebug() << QString("TS_Network >> slotReadClient(); >> ") + (clientSocket->readAll()+"\n\r");

        //create text, which will be sent to client
        QTextStream os(clientSocket);

        //autoDetectUnicode
        os.setAutoDetectUnicode(true);

        //generate html page from QString (includes into <body>...</body> and added headers)
        //but before it, generate one QString from our process in func generateStringFromList();
        os << generateHTML(this->generateStringFromList());
    }
}

QString TS_Network::generateHTML(QString buf)
{
    qDebug() << "TS_Network >> generateHTML(QString buf);";

    //style - CSS (in QString formate)
    QString style;

    //generate style for page
    style = QString("") +
            "body{" +
            "background-color: gray;" +
            "}" +
            "table{" +
            "background-color: white;" +
            "border: 3px solid black;" +
            "border-radius: 10px;" +
            "padding: 5px;" +
            "transition: background-color 400ms;" +
            "margin: 0 auto;" +
            "}" +
            "table tr{" +
            "transition: background-color 400ms, color 400ms;" +
            "}" +
            "table tr:hover{" +
            "background-color: black;" +
            "color: white;"
            "}";

    //generate all html page and add style and message buf (QString, parametr of this func)
    QString response_body = QString("<html>")
            + "<head>"
            + "<title>Test C++ HTTP Server</title>"
            + "<style>"
            + style
            + "</style>"
            + "</head>"
            + "<body>"
            + buf
            + "</body></html>";

    //generate response to a client with headers
    QString response = QString("HTTP/1.1 200 OK\r\n")
            + QString("Version: HTTP/1.1\r\n")
            + QString("Content-Type: text/html; charset=utf-8\r\n")
            + QString("Content-Length: ") + QString::number(response_body.length())
            + QString("\r\n\r\n")
            + response_body;

    //return the result
    return response;
}
