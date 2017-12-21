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
#include<QProcess>
#include<QUdpSocket>
#include<QCommandLineOption>
#include<QCommandLineParser>
#include"IPMsg.h"




extern QReadWriteLock rwLock;

class Iter;

class MainWin : public QObject
{
    Q_OBJECT
private:
    explicit MainWin();
    MyTcpSever *server;
    int socketDescriptor;
    Iter *iter;
    MyThread *iterThread;
    static std::map<std::string,QTcpSocket*> ipMap;
    std::map<QString,QProcess *> pObjectProcess;//进程指针容器
    QUdpSocket *udpSock;//UDP接收套接字
    QList<QString> ipList;//储存所有广播上线得到IP地址
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
    static std::map<std::string,QTcpSocket*> GetIpMap(){
        return ipMap;
    }
public:
    static MainWin *m_pInstance;
private:
    bool    IsIpExist(const QString &qstr);
    void    SendControlCommand(const QString &iPAddr,const char *pCmd);
    void    StartSendProcess(const QStringList &qslt);
    void    InitBroadcast();
    void    BroadCast(ULONG mode);//向局域网广播消息
public slots:
    void    EvReceiveCommand();
    void    EvNewConnection(qintptr ptr1);
    void    EvProStart();//启动了子进程
    void    EvProExit();//退出了子进程
    bool    EvConTcp(QString qstrIp);//连接指定的IP地址
    void    EvSendFile(QString qstrIpAddr,QStringList qstrContext);
    void    EvLeaveProc();//
    void    EvUdp();//对UDP数据报进行处理
    void    EvReFresh();//刷新列表
signals:
    void    Sig();
public:
    void    MakeMsg(char *buf,ULONG command);//制作消息包
    void    MsgBrEntry(const ULONG mode,const QHostAddress &_ip);
    void    MsgAnsEntry();
    bool    FilterGetIp(const QString &_ip);
    void    AnswerMsg(const ULONG mode,const QHostAddress &_ip);//回复消息

};


#endif // MAINWIN_H
