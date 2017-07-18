#include "server.h"
#include<string>
#include<QProcess>
#include<QByteArray>
#include<QFile>
#include<QByteArray>
#include<QCoreApplication>
#include<Windows.h>


Server::Server()
{

    m_pSocket = NULL;
    server  = NULL;
    server = new MyTcpSever;


    socketDescriptor = 0;
   if (!server->isListening()){//监听
       if (server->listen(QHostAddress::Any, 7777)){
           std::cout<<"Open listen port success!"<<std::endl;
       }
       else{ std::cout<<"open listen port fail!"<<std::endl;}
   }
   else{std::cout<<"Function  isListening()  error!"<<std::endl; }
    connect(server, SIGNAL(newClientConnection(qintptr)), this, SLOT(newConnectionSlot(qintptr)));//当有新的连接的时候，就会执行槽函数
  //qDebug()<< QThread::currentThreadId()<<"MAIN";
}




Server::~Server()
{
   if(server){
   delete server;
   server = NULL;
   }
   if(m_pSocket){
       delete m_pSocket;
   m_pSocket = NULL;
   }

   if(conmunicateThread){
       delete conmunicateThread;
       conmunicateThread = NULL;
   }

   if(inputer){
       delete inputer;
       inputer = NULL;
   }
}


void Server::newConnectionSlot(qintptr ptr1){


    //创建套接字用于接收数据
    socketDescriptor = ptr1;
    m_pSocket = new QTcpSocket;
    m_pSocket->setSocketDescriptor(socketDescriptor);
    connect(m_pSocket,SIGNAL(readyRead()),this,SLOT(receiveData()));
    connect(this,SIGNAL(startSendProcess()),this,SLOT(sendProcess()));
    //当服务端执行到此处的时候就已经建立了连接

    conmunicateThread = new MyThread;//  //创建新的线程
    inputer = new CmdBase; //创建输入器对象
    QObject::connect(inputer,SIGNAL(hasNewInput(QString)),this,SLOT(sendCmd(QString)));
    inputer->moveToThread(conmunicateThread);
    QObject::connect(this, SIGNAL(sig()), inputer, SLOT(inputCmd())); // 这里的slot()函数，相当于run()函数的作用
    conmunicateThread->start();
    emit sig(); //运行子线程
}


 //发送命令
 void Server::sendCmd(QString strCmd)
 {
    std::string temp = strCmd.toStdString();

    char *buf=new char[strlen(temp.c_str())+1];//分配一个块内存空间
    strcpy(buf, temp.c_str());//把读进来的字符串转换成 char*类型
    char seg[] = " ";//定义分割符
    char charlist[50][50]={""};//创建一个数组用来存放分割后的字符串数据
    int i =0;
    char *substr= strtok(buf, seg);///?
    while(substr != NULL) {
        strcpy(charlist[i],substr);//把这个字符串放到二维数组的第一行
        i++;

        substr = strtok(NULL,seg);
    }


   // std::cout<<charlist[0]<<std::endl;
   // std::cout<<charlist[1]<<std::endl;
   //std::cout<<charlist[2]<<std::endl;

    QString program1 = "G:/qt/build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
    QStringList arguments1;//启动线程所需要的参数
    arguments1 <<charlist[0]<<charlist[1]<<charlist[2]; //添加参数// s 5000 G:/music
    QProcess::startDetached(program1,arguments1);

    m_pSocket->write(charlist[1]) ;//把端口号扔给接收端
    m_pSocket->waitForBytesWritten();
 }


 //发送文件进程
 void Server::sendProcess()
 {

     //qDebug()<<"This is sendProcess!";
     // QString program1 = "../build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
     QString program1 = "G:/qt/build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
     QStringList arguments1;//启动线程所需要的参数
     arguments1 <<"s"<<"5000"<<"G:/music"; //添加参数// s 5000 G:/music
     QProcess::startDetached(program1,arguments1);
 }



 //接收命令
void Server::receiveData()
{

//   QByteArray temp1;
//   temp1 =  m_pSocket->readAll();
//   qDebug()<<temp1<<"Lenth:"<<temp1.length();
//   if(temp1 == "AS"){
//        emit   startSendProcess();//发射启动进程的信号
//   }

   QByteArray temp1 =  m_pSocket->readAll();
   QString program1 = "G:/qt/build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
   QStringList arguments1;//启动线程所需要的参数
   arguments1 <<"c"<<"127.0.0.1"<<temp1.toStdString().c_str(); //添加参数
   QProcess::startDetached(program1,arguments1);


}
