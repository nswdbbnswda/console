#ifndef ITER_H
#define ITER_H
#include"client.h"
#include"server.h"
#include<iostream>
#include<stdio.h>
#include"cmdbase.h"
#define IMHELP    \
  "Commands: help(h) list(ls) talk(tk) sendfile(sf)\n"\
  "Commands: getfile(gf) refresh(rf) ceaseSend(cs) quit(q)\n"


class Iter : public QObject
{
    Q_OBJECT
public:
    Iter();
    ~Iter();
     void interacter();//用户交互
     void transfStr(char *dest, int flag);
private:

    Client *client;
    Server *server;
    CmdBase *clientInputCmd;
    CmdBase *serverInputCmd;
};


#endif // ITER_H
