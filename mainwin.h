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

#include<QCommandLineOption>
#include<QCommandLineParser>

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
    void    StartSendProcess(const QStringList &qslt );
public slots:
    void    EvReceiveCommand();
    void    EvNewConnection(qintptr ptr1);
    void    EvProStart();//启动了子进程
    void    EvProExit();//退出了子进程
    bool    EvConTcp(QString qstrIp);//连接指定的IP地址
    void    EvSendFile(QString qstrIpAddr,QStringList qstrContext);
    void    EvLeaveProc();//离线处理
signals:

    void    Sig();
};

#endif // MAINWIN_H
