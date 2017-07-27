#include <QCoreApplication>
#include<iostream>
#include<QString>
#include<QProcess>
#include<QTcpServer>
#include "server.h"
#include"client.h"
#include"iter.h"
#include<QObject>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

     Server::getInstance();
     return a.exec();
}
