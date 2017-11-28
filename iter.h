#ifndef ITER_H
#define ITER_H
#include<iostream>
#include<stdio.h>
#include"cmdbase.h"
#include"mainwin.h"
#include<QTcpSocket>
#include"mytcpsever.h"
#include <QCoreApplication>
#include<QMutex>
#include<QReadWriteLock>
#include<windows.h>
#define IMHELP    \
  "Commands: help(h) list(ls) talk(tk) sendfile(sf)\n"\
  "Commands: getfile(gf) refresh(rf) ceaseSend(cs) quit(q)\n"

#define VERSION  ("version0.11")


class Iter : public QObject
{
    Q_OBJECT
public:
    explicit Iter();
    virtual ~Iter();
    void    TransfStr(char *dest, int flag);
    bool    IsFindSuc(std::map<std::string,int> map,std::string key);//判断map里面是否有key
public:
    MyThread *addConThread;
signals:
    void    MainWinSend(QString);
    void    SigAddTcpThread();
    void    AddTcp(QString);
    void    IpChat(QString,QString);
public slots:
    void    Interacter();//用户交互
};


#endif // ITER_H
