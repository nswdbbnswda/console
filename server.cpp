#include "server.h"
#include<string>
#include<QProcess>
#include<QByteArray>
#include<QFile>
#include<QByteArray>
#include<QCoreApplication>
#include<Windows.h>
#include"iter.h"
std::map<std::string,int> Server::ipMap;
QReadWriteLock rwLock;
Server::Server()
{
    iCurNum = 0;
    server  = NULL;
    server = new MyTcpSever;
    socketDescriptor = 0;
    if (!server->isListening()){//监听
        if (!server->listen(QHostAddress::AnyIPv4, 5000)){
            std::cout<<"open listen port fail!"<<std::endl;//如果监听失败,打出这条提示消息
        }
    }
    else{std::cout<<"Function  isListening()  error!"<<std::endl; }
    QObject::connect(server, SIGNAL(newClientConnection(qintptr)), this, SLOT(newConnectionSlot(qintptr)));//当有新的连接的时候，就会执行槽函数


    iter = new Iter;//创建交互对象
    iterThread = new MyThread;//  //创建新的线程
    iter->moveToThread(iterThread);
    QObject::connect(iter,SIGNAL(ipChat(QString,QString)),this,SLOT(ipChatSlot(QString,QString)));//聊天功能的信号与槽
    QObject::connect(iter,SIGNAL(addTcp(QString)),this,SLOT(conTcp(QString)));//当执行cad命令的时候，就执行连接TCP动作
    QObject::connect(this, SIGNAL(sig()), iter, SLOT(interacter())); // 这里的slot()函数，相当于run()函数的作用
    QObject::connect(iter,SIGNAL(serverSend(QString)),this,SLOT(sendCmd(QString)));//当iter对象发出发信号的的时候响应发送命令槽
    iterThread->start();
    emit sig(); //运行子线程

//    rwLock.lockForWrite();
//    qDebug()<<QThread::currentThreadId()<<"main";
    //    rwLock.unlock();
}


//判断容器中是否有你要查找的IP地址
bool Server::isIpExist(const QString &qstr)
{
    std::map<std::string,int>::iterator it = ipMap.find(qstr.toStdString().c_str());//看看map中是否有这个地址
    if(it!=ipMap.end()){//如果找map中有这个IP地址
    return true;
    }
    else{
        return false;
    }
}

Server::~Server()
{

    if(server){
        delete server;
        server = NULL;
    }


    if(iterThread){
        delete iterThread;
        iterThread = NULL;
    }


    if(iter){
        delete iter;
        iter = NULL;
    }
}


//显示所有的IP连接列表,所有连接皆为对侧IP地址
bool Server::showIpList()
{
    if(ipMap.empty()){
        return false;
    }
    for(auto &w:ipMap){
        std::cout<<w.first<<std::endl;//把map中的所有IP地址都打印出来

    }
    return true;
}

//被动连接TCP
void Server::newConnectionSlot(qintptr ptr1){
    QHostAddress ipaddr;
    //创建套接字用于接收数据
    socketDescriptor = ptr1;
    m_pSocket[iCurNum].setSocketDescriptor(socketDescriptor);//设置套接字描述符
    // m_pSocket->setSocketDescriptor(socketDescriptor);
    ipaddr = m_pSocket[iCurNum].peerAddress();//获取客户端的IP地址
    ipMap[ipaddr.toString().toStdString()] = iCurNum;//把IP地址对应的索引号存到map里中
    //把新来的这个套接字和读数据槽连接起来
    connect(&m_pSocket[iCurNum],SIGNAL(readyRead()),this,SLOT(receiveData()));//当一个服务端请求连接的时候 把一个新的套接字和接收槽连接上
    iCurNum++;


    //   connect(this,SIGNAL(startSendProcess()),this,SLOT(sendProcess()));
    //当服务端执行到此处的时候就已经建立了连接
    //    conmunicateThread = new MyThread;//  //创建新的线程
    //    inputer = new CmdBase; //创建输入器对象
    //    QObject::connect(inputer,SIGNAL(hasNewInput(QString)),this,SLOT(sendCmd(QString)));
    //    inputer->moveToThread(conmunicateThread);
    //    QObject::connect(this, SIGNAL(sig()), inputer, SLOT(inputCmd())); // 这里的slot()函数，相当于run()函数的作用
    //    conmunicateThread->start();
    //    emit sig(); //运行子线程
}


//发送命令
void Server::sendCmd(QString strCmd)
{
    std::string numSocket = strCmd.toStdString();//192.168.253.19 //客户端的地址

    std::string sendContext("Hello!");
    //找到这个套接字,并且发送"Hello!"
    m_pSocket[ipMap[numSocket]].write(sendContext.c_str());
    m_pSocket[ipMap[numSocket]].waitForBytesWritten();//如果有找到套接字

    // std::string str1("Hello,client!");

    // std::cout<<"Server accept!"<<temp<<std::endl;

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

    //    QString program1 = "G:/qt/build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
    //    QStringList arguments1;//启动线程所需要的参数
    //    arguments1 <<charlist[0]<<charlist[1]<<charlist[2]; //添加参数// s 5000 G:/music
    //    QProcess::startDetached(program1,arguments1);

    //    m_pSocket->write(charlist[1]) ;//把端口号扔给接收端
    //    m_pSocket->waitForBytesWritten();
    // m_pSocket[iCurNum].write(str1.c_str());
    // m_pSocket[iCurNum].waitForBytesWritten();

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




//主动连接TCP
 void Server::conTcp(QString qstrIp)
 {

     if(isIpExist(qstrIp)){//如果要添加的IP地址已经存在了

         std::cout<<"The address already exists";//提示已经存在

         return;//返回,不进行添加操作
     }
     //如果要添加的目标地址是一个新的IP地址
     m_pSocket[iCurNum].connectToHost(qstrIp.toStdString().c_str(),5000);//尝试用这个连接这个IP地址
     if(m_pSocket[iCurNum].waitForConnected()){//如果连接成功

     std::cout<<"Successfully connected to "<<qstrIp.toStdString().c_str()<<std::endl;//提示连接成功

     QObject::connect(&m_pSocket[iCurNum],SIGNAL(readyRead()),this,SLOT(receiveData()));//把每一个主动连接的套接字都连接到接收槽上
     //如果连接成功把这个IP地址记录下来
     ipMap[qstrIp.toStdString()] = iCurNum;    //这个地址所对应的套接字就是m_oSocekt[iCurNum]
     iCurNum++;
     }
     else{//连接失败了

     std::cout<<"Failed to connect to  "<<qstrIp.toStdString().c_str()<<std::endl;

    }

 }


//聊天
 void Server::ipChatSlot(QString qstrIpAddr,QString qstrContext)
 {
   if(!isIpExist(qstrIpAddr)){//如果map中没有这个地址,退出聊天模块
       std::cout<<"The address does not exist"<<std::endl;//提示地址不存在,不能进行聊天
       return;
   }
  std::string chatContext;//聊天内容
  chatContext = qstrContext.toStdString();

   char *buf = new char[strlen(chatContext.c_str())+1];//分配一个块内存空间
   strcpy(buf, chatContext.c_str());//把读进来的字符串转换成 char*类型
   char seg[] = " ";//定义分割符
   char charlist[50][50]={""};//创建一个数组用来存放分割后的字符串数据
   int i =0;
   char *substr= strtok(buf, seg);///?
   while(substr != NULL) {
          strcpy(charlist[i],substr);//把这个字符串放到二维数组的第一行
          i++;

         substr = strtok(NULL,seg);
   }

   QString program1 = "D:/AutoSend/AutoSend.exe";//要启动的另外一个进程的路径
   QStringList arguments1;//启动线程所需要的参数
   arguments1 <<charlist[0]<<charlist[1]<<charlist[2]; //添加参数// s 5000 G:/music
   QProcess::startDetached(program1,arguments1);

  //    m_pSocket->write(charlist[1]) ;//把端口号扔给接收端
  //    m_pSocket->waitForBytesWritten();
  // m_pSocket[iCurNum].write(str1.c_str());
  // m_pSocket[iCurNum].waitForBytesWritten();

  std::string numSocket = qstrIpAddr.toStdString();//获得套接字号码
  m_pSocket[ipMap[numSocket]].write(charlist[1]);//把端口号扔给接收端
  m_pSocket[ipMap[numSocket]].waitForBytesWritten();//等待发送完毕


 }


 //接收数据
 void Server::receiveData()
 {
     QTcpSocket *socket = (QTcpSocket *)sender();//获得信号的发送者
     QHostAddress ipaddr = socket->peerAddress();//获取客户端的IP地址
     QByteArray temp1;
     temp1 = socket->readAll();
     socket->waitForBytesWritten();
    // QString program1 = "G:/qt/build-AutoSend-Desktop_Qt_5_6_2_MSVC2013_64bit-Release/release/AutoSend.exe";//要启动的另外一个进程的路径
      QString program1 = "D:/AutoSend/AutoSend.exe";//要启动的另外一个进程的路径
     QStringList arguments1;//启动线程所需要的参数
     arguments1 <<"c"<<ipaddr.toString()<<temp1.data(); //添加参数
     QProcess::startDetached(program1,arguments1);
 }
