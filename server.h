#ifndef SERVER_H
#define SERVER_H
#include<QObject>
#include<QTcpServer>
#include<iostream>
#include<QTcpSocket>
#include<QByteArray>
#include"mytcpsever.h"
#include"mythread.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server();
    virtual ~Server();
private:
    MyTcpSever *server;
    QTcpSocket *m_pSocket;
    int socketDescriptor;
    MyThread *conmunicateThread;
    CmdBase *inputer;

public slots:
    void receiveData();
    void newConnectionSlot(qintptr ptr1);
    void sendCmd(QString strCmd);
    void sendProcess();

signals:
    void  startSendProcess();//启动发送文件进程
    void sig();




};

#endif // SERVER_H
