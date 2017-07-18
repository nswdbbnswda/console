#ifndef CLIENT_H
#define CLIENT_H
#include<QHostAddress>
#include<QDebug>
#include<QTcpSocket>
#include<iostream>
#include"cmdbase.h"
#include"mythread.h"



class Client : QObject
{Q_OBJECT
public:
    explicit Client();
    virtual~Client();
private:
    QTcpSocket *m_pSocket;
    qint32 num,CurrentNum,TotalNum,LastBlock,NameLength;
    char *ReceiveHead;
    qint32 port,Flag,TotalByte,WrittenByte;
    std::string ipAddr;
    qint64 FileNumber;
    MyThread *conmunicateThreadClient;
    CmdBase *cmdInputer;


signals:
    void startSendProcess();
    void sig();
public slots:
    void receiveData();//接收数据
    void sendProcess();
    void sendCmd(QString strCmd);



};

#endif // CLIENT_H
