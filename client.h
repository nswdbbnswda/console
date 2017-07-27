#ifndef CLIENT_H
#define CLIENT_H
#include<QHostAddress>
#include<QDebug>
#include<QMutex>
#include<QTcpSocket>
#include<iostream>
#include"cmdbase.h"
#include"mythread.h"
#include<QWriteLocker>

class Client : public QObject
{Q_OBJECT
public:
    explicit Client();
    virtual~Client();
private:
    //QTcpSocket *m_pSocket;
    qint32 num,CurrentNum,TotalNum,LastBlock,NameLength;
    char *ReceiveHead;
    qint32 port,Flag,TotalByte,WrittenByte;
    std::string ipAddr;
    qint64 FileNumber;
    MyThread *conmunicateThreadClient;
    CmdBase *cmdInputer;
    QTcpSocket m_pSocket[100];//创建100个套接字;
signals:

    void startSendProcess();
public slots:
    void receiveData();//接收数据
    void sendProcess();
    void sendCmd(QString strCmd);
    void addNewTcp();
    void slot(QString qstr);
};


#endif // CLIENT_H
