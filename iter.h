#ifndef ITER_H
#define ITER_H
#include"client.h"
#include<iostream>
#include<stdio.h>
#include"cmdbase.h"
#include"server.h"
#include<QTcpSocket>
#include"mytcpsever.h"
#include <QCoreApplication>
#include<QMutex>
#include<QReadWriteLock>
#include<windows.h>
#define IMHELP    \
  "Commands: help(h) list(ls) talk(tk) sendfile(sf)\n"\
  "Commands: getfile(gf) refresh(rf) ceaseSend(cs) quit(q)\n"




class Iter : public QObject
{
    Q_OBJECT
public:
    explicit Iter();
    virtual ~Iter();
     void transfStr(char *dest, int flag);
    bool isFindSuc(std::map<std::string,int> map,std::string key);//判断map里面是否有key
    MyThread *addConThread;
signals:
    void serverSend(QString);
    void sigAddTcpThread();
    void addTcp(QString);
    void ipChat(QString,QString);
private:


public slots:
    void interacter();//用户交互
};


#endif // ITER_H
