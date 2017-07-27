#include "client.h"
#include<QDebug>
#include<iostream>
#include<QByteArray>
#include<string>
#include<QProcess>
#include<Windows.h>




Client::Client()
{

//    std::cout<<"IP:";
//     std::cin>>ipAddr;
//    //ipAddr = std::string("127.0.0.1");
//    m_pSocket = new QTcpSocket();//创建客户端套接字
//    m_pSocket->connectToHost(QHostAddress(ipAddr.c_str()),5000);//发起连接

//    connect(m_pSocket, SIGNAL(readyRead()),this, SLOT(receiveData())); //连接服务器
//    connect(this,SIGNAL(startSendProcess()),this,SLOT(sendProcess()));//当槽收到启动信号的时候就启动发射进程

//    if(m_pSocket->waitForConnected()){

//        std::cout<<"connected to "<<ipAddr<<" success!"<<std::endl;
//        conmunicateThreadClient = new MyThread;    //创建发送命令线程
//        cmdInputer = new CmdBase;   //创建发送器对象
//        QObject::connect(cmdInputer,SIGNAL(hasNewInput(QString)),this,SLOT(sendCmd(QString)));
//        cmdInputer->moveToThread(conmunicateThreadClient);//移动对象到子线程
//        QObject::connect(this, SIGNAL(sig()), cmdInputer, SLOT(inputCmd())); // 这里的slot()函数，相当于run()函数的作用
//        conmunicateThreadClient->start();
//        emit sig();
//    }
//    else{
//        std::cout<<"connected to "<<ipAddr<<"fail!"<<std::endl;
//    }

}




Client::~Client()
{
//    if(m_pSocket) {
//        delete m_pSocket;
//    m_pSocket = NULL;
//    }

//    if(conmunicateThreadClient){
//        delete conmunicateThreadClient;
//        conmunicateThreadClient = NULL;
//    }

//    if(cmdInputer){
//        delete cmdInputer;
//        cmdInputer = NULL;
//    }

}


//接收命令并启动发送进程
void Client::receiveData()
{
   //QByteArray temp1 =  m_pSocket->readAll();
   // qDebug()<<temp1;
//    QString program1 = "G:/qt/build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
//    QStringList arguments1;//启动线程所需要的参数
//    arguments1 <<"c"<<"127.0.0.1"<<temp1.toStdString().c_str(); //添加参数
//    QProcess::startDetached(program1,arguments1);
}




void Client::sendProcess(){
//    qDebug()<<"This is sendProcess!";
//    // QString program1 = "../build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
//    QString program1 = "G:/qt/build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
//    QStringList arguments1;//启动线程所需要的参数
//    arguments1 <<"s"<<"G:/music"; //添加参数
//    QProcess::startDetached(program1,arguments1);
}





//发送命令到服务端
void Client::sendCmd(QString  strCmd)
{
    std::string temp = strCmd.toStdString();

//    char *buf=new char[strlen(temp.c_str())+1];//分配一个块内存空间
//    strcpy(buf, temp.c_str());//把读进来的字符串转换成 char*类型
//    char seg[] = " ";//定义分割符
//    char charlist[50][50]={""};//创建一个数组用来存放分割后的字符串数据
//    int i =0;
//    char *substr= strtok(buf, seg);///?
//    while(substr != NULL) {
//        strcpy(charlist[i],substr);//把这个字符串放到二维数组的第一行
//        i++;

//        substr = strtok(NULL,seg);
//    }

//    //启动发送进程
//    QString program1 = "G:/qt/build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
//    QStringList arguments1;//启动线程所需要的参数
//    arguments1 <<charlist[0]<<charlist[1]<<charlist[2]; //添加参数// s 5000 G:/music
//    QProcess::startDetached(program1,arguments1);

//    m_pSocket->write(charlist[1]);//把端口号扔过去给接收端
//    m_pSocket->waitForBytesWritten();//等待发送完毕



   // m_pSocket->write(temp.c_str());//
    //m_pSocket->waitForBytesWritten();//等待发送完毕


}





void Client::addNewTcp()
{
//     rwLock.lockForWrite();
//     qDebug()<<QThread::currentThreadId()<<"Client";//打印一下当前线程的ID号
//     rwLock.unlock();

}


void Client::slot(QString qstr )
{

//    std::string ipAddr;
//    ipAddr =  qstr.toStdString();
//  rwLock.lockForWrite();
//  qDebug()<<QThread::currentThreadId()<<"slot";
//  rwLock.unlock();

//    m_pSocket[0].connectToHost(QHostAddress(ipAddr.c_str()),5000);//发起连接
//      if(m_pSocket[0].waitForConnected()){
//           rwLock.lockForWrite();
//        std::cout<<"connect success!"<<std::endl;
//        rwLock.unlock();
//      }
//     else{
//       rwLock.lockForWrite();
//       std::cout<<"connect fail!"<<std::endl;
//       rwLock.unlock();
//      }

}







