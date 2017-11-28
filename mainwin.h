#ifndef MAINWIN_H
#define MAINWIN_H
#include<QObject>
#include<QTcpServer>
#include<iostream>
#include<QTcpSocket>
#include<QByteArray>
#include"mytcpsever.h"
#include"mythread.h"
#include<QReadWriteLock>

extern QReadWriteLock rwLock;

class Iter;

class MainWin : public QObject
{
    Q_OBJECT  
private:
    explicit MainWin();
    MyTcpSever *server;
    QTcpSocket m_pSocket[100];
    int socketDescriptor;
    Iter *iter;
    MyThread *iterThread;
    static std::map<std::string,int> ipMap;
    int iCurNum;
public:
    virtual ~MainWin();
    static MainWin *GetInstance()
    {
        static MainWin *m_pInstance;
        if(m_pInstance == NULL){
            m_pInstance = new MainWin;
        }
        return m_pInstance;
    }
    bool    ShowIpList();
    static std::map<std::string,int> GetIpMap(){
        return ipMap;
    }
public:
    static MainWin *m_pInstance;
private:
    bool    IsIpExist(const QString &qstr);
public slots:
    void    EvReceiveData();
    void    EvNewConnectionSlot(qintptr ptr1);
    void    EvSendCmd(QString strCmd);
    void    EvStartSendProcess();
    void    EvConTcp(QString qstrIp);//连接指定的IP地址
    void    IpChatSlot(QString qstrIpAddr,QString qstrContext);
signals:
    void    StartSendProcess();//启动发送文件进程
    void    Sig();
};

#endif // MAINWIN_H
