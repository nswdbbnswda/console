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

 qRegisterMetaType<string>("string");

    //启动发射进程
  //  QString program1 = "../build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
  //  QStringList arguments1;//启动线程所需要的参数
   // arguments1 <<"s"<<"G:/music"; //添加参数
   // QProcess::startDetached(program1,arguments1);

    //启动接收进程
   // QString program2 = "../build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
   // QStringList arguments2;//启动线程所需要的参数
   // arguments2 <<"c"<<"127.0.0.1"; //添加参数
    //QProcess::startDetached(program2,arguments2);

   //  Server server;
    // Client client;

  Iter iter;
  iter.interacter();

   return a.exec();
}
