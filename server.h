#ifndef SERVER_H
#define SERVER_H
#include<QObject>
#include<QTcpServer>
#include<iostream>
#include<QTcpSocket>
#include<QByteArray>
#include"mytcpsever.h"
#include"mythread.h"
#include"client.h"
extern QReadWriteLock rwLock;

class Iter;

class Server : public QObject
{
    Q_OBJECT
public:
    virtual ~Server();
    static Server *getInstance()
    {
        static Server *m_pInstance;
        if(m_pInstance == NULL){
            m_pInstance = new  Server;
        }
        return m_pInstance;
    }
    bool showIpList();
    static std::map<std::string,int> getIpMap(){
        return ipMap;
    }

public:
    static Server *m_pInstance;
private:
    explicit Server();
    MyTcpSever *server;
    QTcpSocket m_pSocket[100];
    int socketDescriptor;
    Iter *iter;
    MyThread *iterThread;
    Client *tcpAdder;
    static std::map<std::string,int> ipMap;
    int iCurNum;
private:
    bool isIpExist(const QString &qstr);
public slots:
    void receiveData();
    void newConnectionSlot(qintptr ptr1);
    void sendCmd(QString strCmd);
    void sendProcess();
    void conTcp(QString qstrIp);//连接指定的IP地址
    void ipChatSlot(QString qstrIpAddr,QString qstrContext);
signals:
    void startSendProcess();//启动发送文件进程
    void sig();

};

#endif // SERVER_H
