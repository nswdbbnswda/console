#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<QObject>
#include<iostream>
#include<QThread>
#include<string>
#include"cmdbase.h"


class MyThread : public QThread
{
public:
    explicit MyThread();
    virtual ~MyThread();
protected:
    virtual void    run();
signals:
    void    HasNewInput(std::string strCmd);
private:
    CmdBase *cmdInputer;

};

#endif // MYTHREAD_H
