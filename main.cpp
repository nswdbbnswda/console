#include <QCoreApplication>
#include<iostream>
#include<QString>
#include<QProcess>
#include<QTcpServer>
#include "mainwin.h"
#include"iter.h"
#include<QObject>




using namespace std;

int main(int argc, char *argv[])
{
     QCoreApplication a(argc, argv);
     MainWin::GetInstance();
     return a.exec();
}
