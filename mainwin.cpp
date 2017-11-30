#include "mainwin.h"
#include<string>
#include<QProcess>
#include<QByteArray>
#include<QFile>
#include<QByteArray>
#include<QCoreApplication>
#include<Windows.h>
#include"iter.h"
std::map<std::string,QTcpSocket*> MainWin::ipMap;
QReadWriteLock rwLock;

MainWin::MainWin()
{
    server  = NULL;
    server = new MyTcpSever;

    socketDescriptor = 0;
    if (!server->isListening()){//监听
        if (!server->listen(QHostAddress::AnyIPv4, 5000)){std::cout<<"open listen port fail!"<<std::endl;}
    }
    else{std::cout<<"Function  isListening()  error!"<<std::endl; }
    QObject::connect(server, SIGNAL(newClientConnection(qintptr)), this, SLOT(EvNewConnection(qintptr)));//当有新的连接的时候，就会执行槽函数
    iter = new Iter;//创建交互对象
    iterThread = new MyThread;//  //创建新的线程
    iter->moveToThread(iterThread);

    //交互信号与槽函数连接区域
    QObject::connect(iter,SIGNAL(SendFile(QString,QString)),this,SLOT(EvSendFile(QString,QString)));//发送文件
    QObject::connect(iter,SIGNAL(AddTcp(QString)),this,SLOT(EvConTcp(QString)));//当执行cad命令的时候，就执行连接TCP动作
    QObject::connect(this, SIGNAL(Sig()), iter, SLOT(Interacter())); // 这里的slot()函数，相当于run()函数的作用


    iterThread->start();
    emit Sig(); //启动命令交互线程
}


//判断容器中是否有要查找的IP地址
bool MainWin::IsIpExist(const QString &qstr)
{
    std::map<std::string,QTcpSocket*>::iterator it = ipMap.find(qstr.toStdString().c_str());//看看map中是否有这个地址
    if(it != ipMap.end()){  return true; }//如果找map中有这个IP地址
    else {return false;}
}


//发送控制指令
void MainWin::SendControlCommand(const QString &iPAddr,const char *pCmd)
{  
    std::string numSocket = iPAddr.toStdString();//获得套接字号码
    ipMap[iPAddr.toStdString()]->write(pCmd);//把端口号扔给接收端
    ipMap[iPAddr.toStdString()]->waitForBytesWritten();//等待发送完毕
}


//析构函数
MainWin::~MainWin()
{
    if(server){delete server;server = NULL;}
    if(iterThread){delete iterThread; iterThread = NULL;}
    if(iter){ delete iter;  iter = NULL; }
}


//显示所有的IP连接列表,所有连接皆为对侧IP地址
bool MainWin::ShowIpList()
{
    if(ipMap.empty()){return false;}
    for(auto &w : ipMap){ std::cout<<w.first<<std::endl; }//把map中的所有IP地址都打印出来(键值)
    return true;
}


//有主机主动连接本主机
void MainWin::EvNewConnection(qintptr ptr1)
{
    QHostAddress ipaddr;
    socketDescriptor = ptr1;
    QTcpSocket *pSocket = new QTcpSocket;//创建套接字用于接收数据
    pSocket->setSocketDescriptor(socketDescriptor);//设置套接字描述符
    ipaddr = pSocket->peerAddress();//获取客户端的IP地址
    ipMap[ipaddr.toString().toStdString()] = pSocket;//把套接字指针加到map中
    connect(ipMap[ipaddr.toString().toStdString()],SIGNAL(readyRead()),this,SLOT(EvReceiveCommand()));//当一个服务端请求连接的时候 把一个新的套接字和接收槽连接上
    connect(ipMap[ipaddr.toString().toStdString()],SIGNAL(disconnected()),this,SLOT(EvLeaveProc()));//断开连接处理
}


//启动发送文件进程
void MainWin::StartSendProcess(const QStringList &qslt )
{

    QString program1 = QCoreApplication::applicationDirPath() + "/AutoSend.exe";//待启动程序路径
    //每启动一个进程都为这个进程创建一个对象
    QProcess *tmp = new QProcess;

    connect(tmp,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(EvProExit()));//子进程启动了
    connect(tmp,SIGNAL(started()),this,SLOT(EvProStart()));//子进程结束了
    tmp->start(program1, qslt);
    tmp->waitForStarted();//等待进程启动
}


//发起TCP连接
bool MainWin::EvConTcp(QString qstrIp)
{
    if(IsIpExist(qstrIp)){//判断连接是否已存在
        std::cout<<"The address already exists!";
        return false;//返回,不进行添加操作
    }
    //建立新连接
    QTcpSocket *pSocket = new QTcpSocket;
    pSocket->connectToHost(qstrIp.toStdString().c_str(),5000);//尝试用这个连接这个IP地址
    if(pSocket->waitForConnected()){//如果连接成功
        std::cout<<"Successfully connected to "<<qstrIp.toStdString().c_str()<<"!"<<std::endl;//提示连接成功
        ipMap[qstrIp.toStdString()] = pSocket;//把这个指针加入到map中
        QObject::connect(ipMap[qstrIp.toStdString()],SIGNAL(readyRead()),this,SLOT(EvReceiveCommand()));//把每一个主动连接的套接字都连接到接收槽上
        QObject::connect(ipMap[qstrIp.toStdString()],SIGNAL(disconnected()),this,SLOT(EvLeaveProc()));//断开连接处理
        return true;
    }
    else{std::cout<<"Failed to connect to  "<<qstrIp.toStdString().c_str()<<"!"<<std::endl; delete pSocket;} //连接失败了
    return false;
}


//处理"sf"事件
void MainWin::EvSendFile(QString qstrIpAddr,QString qstrContext)
{
    if(!IsIpExist(qstrIpAddr)){//如果map中没有这个地址,退出
        std::cout<<"The address does not exist"<<std::endl;//提示地址不存在，不能进行命令发送
        return;
    }
    std::string  CommandContext;//命令内容
    CommandContext = qstrContext.toStdString();
    char *buf = new char[strlen( CommandContext.c_str())+1];//分配一个块内存空间
    strcpy(buf,  CommandContext.c_str());//把读进来的字符串转换成 char*类型
    char seg[] = " ";//定义分割符
    char charlist[50][50]={""};//创建一个数组用来存放分割后的字符串数据
    int i =0;
    char *substr= strtok(buf, seg);
    while(substr != NULL) {
        strcpy(charlist[i],substr);//把这个字符串放到二维数组的第一行
        i++;
        substr = strtok(NULL,seg);
    }
    QStringList arguments1;
    arguments1 <<charlist[0]<<charlist[1]<<charlist[2]; //添加参数// s 5000 G:/music
    StartSendProcess(arguments1);//启动本地文件发送进程
    SendControlCommand(qstrIpAddr,charlist[1]);//发送端口号
    delete[] buf;
}


//离线处理
void MainWin::EvLeaveProc()
{
    QTcpSocket *socket = (QTcpSocket *)sender();//获得信号的发送者
    QHostAddress ipaddr = socket->peerAddress();//获取IP地址

    delete ipMap[ipaddr.toString().toStdString()];//释放套接字
    ipMap.erase(ipaddr.toString().toStdString());//把这个元素从map中移除  //内存尚未释放
}



//子进程启动了
void MainWin::EvProStart()
{
 qDebug()<<"正在传输文件...";
}




//子进程退出了
void MainWin::EvProExit()
{
    QProcess *tmp = (QProcess *)sender();//获得信号的发送者指针
    delete tmp;
    qDebug()<<"文件传输完毕!";
}


//接收指令
void MainWin::EvReceiveCommand()
{
    QTcpSocket *socket = (QTcpSocket *)sender();//获得信号的发送者
    QHostAddress ipaddr = socket->peerAddress();//获取客户端的IP地址
    QByteArray temp1;
    temp1 = socket->readAll();

    QStringList arguments1;//启动线程所需要的参数
    arguments1 <<"c"<<ipaddr.toString()<<temp1.data(); //添加参数
    StartSendProcess(arguments1);//启动接收进程
}




