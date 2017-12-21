#include "mythread.h"
#include<QDebug>

MyThread::MyThread()
{


}

MyThread::~MyThread()
{


}


//这个线程启动了以后，就是等待输入信息，然后把输入的信息发送给MainWin 对象
void MyThread::run()
{
   exec();
}



