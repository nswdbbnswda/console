#include "mainwin.h"
#include<string>
#include<QProcess>
#include<QByteArray>
#include<QFile>
#include<QByteArray>
#include<QCoreApplication>
#include"iter.h"
#include<QTime>
#include<QList>
#include<QNetworkInterface>


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
    QObject::connect(iter,SIGNAL(SendFile(QString,QStringList)),this,SLOT(EvSendFile(QString,QStringList)));//发送文件
    QObject::connect(iter,SIGNAL(ReFresh()),this,SLOT(EvReFresh()));//刷新用户列表
    QObject::connect(iter,SIGNAL(AddTcp(QString)),this,SLOT(EvConTcp(QString)));//当执行cad命令的时候，就执行连接TCP动作
    QObject::connect(this, SIGNAL(Sig()), iter, SLOT(Interacter())); // 这里的slot()函数，相当于run()函数的作用

    iterThread->start();


    InitBroadcast();//初始化广播上线系统
    BroadCast(IPMSG_BR_ENTRY);//向局域网发送广播上线消息

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
    if(udpSock){ delete udpSock; udpSock = NULL;}
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
    //    //每启动一个进程都为这个进程创建一个对象
    //    QProcess *tmp = new QProcess;
    //    connect(tmp,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(EvProExit()));//子进程启动了
    //    connect(tmp,SIGNAL(started()),this,SLOT(EvProStart()));//子进程结束了
    //    tmp->start(program1, qslt);
    //    tmp->waitForStarted();//等待进程启动
    QProcess::startDetached(program1,qslt);//启动AutoSend
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
        //std::cout<<"Successfully connected to "<<qstrIp.toStdString().c_str()<<"!"<<std::endl;//提示连接成功
        ipMap[qstrIp.toStdString()] = pSocket;//把这个指针加入到map中
        QObject::connect(ipMap[qstrIp.toStdString()],SIGNAL(readyRead()),this,SLOT(EvReceiveCommand()));//把每一个主动连接的套接字都连接到接收槽上
        QObject::connect(ipMap[qstrIp.toStdString()],SIGNAL(disconnected()),this,SLOT(EvLeaveProc()));//断开连接处理
        return true;
    }
    else{std::cout<<"Failed to connect to  "<<qstrIp.toStdString().c_str()<<"!"<<std::endl; delete pSocket;} //连接失败了
    return false;
}


//启动发送文件进程以及给对侧主机发送用于发送数据的端口号
void MainWin::EvSendFile(QString qstrIpAddr,QStringList qstrContext)
{
    if(!IsIpExist(qstrIpAddr)){//如果map中没有这个地址,退出
        std::cout<<"The address does not exist"<<std::endl;//提示地址不存在，不能进行命令发送
        return;
    }
    QString qsPort;
    QStringList arguments1;
    arguments1<<qstrContext;

    qsPort = arguments1.at(arguments1.indexOf("-p") + 1);//找到端口号
    StartSendProcess(arguments1);//启动本地文件发送进程
    SendControlCommand(qstrIpAddr,qsPort.toStdString().data());//发送端口号
}


//离线处理
void MainWin::EvLeaveProc()
{
    QTcpSocket *socket = (QTcpSocket *)sender();//获得信号的发送者
    QHostAddress ipaddr = socket->peerAddress();//获取IP地址

    delete ipMap[ipaddr.toString().toStdString()];//释放套接字
    ipMap.erase(ipaddr.toString().toStdString());//把这个元素从map中移除  //内存尚未释放
    ipList.removeAt(ipList.indexOf(ipaddr.toString()));//把UDP地址链表中的该下线主机地址也给清理掉
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
    //接收短消息 并且解析出IP地址和端口号码
    QTcpSocket *socket = (QTcpSocket *)sender();//获得信号的发送者
    QHostAddress ipaddr = socket->peerAddress();//获取客户端的IP地址
    QByteArray temp1;
    temp1 = socket->readAll();

    //启动接收进程
    QStringList arguments1;//默认保存到应用程序根目录
    arguments1 <<"-c"<<"-i"<<ipaddr.toString()<<"-p"<<temp1.data()<<"-d"<<QCoreApplication::applicationDirPath() + "/DOWNLOAD"; //添加启动参数 -c  -i 127.0.0.1  -p 5002 -d G:/DE
    StartSendProcess(arguments1);//启动接收进程
}




//初始化UDP
void MainWin::InitBroadcast()
{
    udpSock = new QUdpSocket(this);//创建一个UDP套接字
    udpSock->bind(DEST_PORT,QUdpSocket::ShareAddress); //绑定LOCAL_PORT端口作为数据输出口
    connect(udpSock,SIGNAL(readyRead()),this,SLOT(EvUdp()));//收到广播消息进行处理
}



//进行广播
void MainWin::BroadCast(ULONG mode)
{
    //制作数据报
    char buf[20];
    MakeMsg(buf,mode);
    QByteArray datagram = QByteArray(buf);
    //发送数据报
    udpSock->writeDatagram(datagram.data(),datagram.size(),QHostAddress::Broadcast,DEST_PORT);//向网络上所有主机的12811端口发送数据
}






//接收数据报
void MainWin::EvUdp()
{
    QHostAddress client_address;//声明一个QHostAddress对象用于保存发送端的信息

    while(udpSock->hasPendingDatagrams())
    {
        quint16 recPort = 0;
        QByteArray datagram;
        datagram.resize(udpSock->pendingDatagramSize());//datagram大小为等待处理数据报的大小才能接收数据;
        udpSock->readDatagram(datagram.data(),datagram.size(), &client_address, &recPort);//接收数据报

        //查看一下是什么指令
        switch(atol(datagram.data()))
        {
        case IPMSG_BR_ENTRY:
        {
            if(FilterGetIp(client_address.toString())){//有网络上的其他主机上线
                MsgBrEntry(IPMSG_ANSENTRY,client_address);//给该主机回复"我在"消息

                if( -1 == ipList.indexOf(client_address.toString())){//看看我的IP列表中没有该IP
                    ipList.append(client_address.toString());//把它添加到我的IP列表中
                  //  qDebug()<<client_address.toString();
                }
            }
        }
            break;
        case IPMSG_ANSENTRY:
            if(FilterGetIp(client_address.toString()))//过滤发给自己的消息
            {
                if( -1 == ipList.indexOf(client_address.toString())){//链表中不存在这个地址
                    ipList.append(client_address.toString());//把地址插入到链表
                    //qDebug()<<client_address.toString();//表示这是网络中处于在线状态的一个主机，并且本机第一次与该机进行建立通讯连接
                    //发起TCP连接
                     EvConTcp(client_address.toString());

                }
            }
            break;
        }
    }

}

//刷新用户列表
void MainWin::EvReFresh()
{
    BroadCast(IPMSG_BR_ENTRY);//向局域网发送广播上线消息
}

//制作消息包
void MainWin::MakeMsg(char *buf,ULONG command)
{
    int			cmd = GET_MODE(command);//把ULONG类型通过宏转换成整型数
    bool		is_br_cmd =	cmd == IPMSG_BR_ENTRY ||//如果命令是IPMSG_BR_ENTRY，IPMSG_BR_EXIT，IPMSG_BR_ABSENCE，
            cmd == IPMSG_BR_EXIT  ||//IPMSG_NOOPERATION四个之中的其中一个该表达式的值就为真
            cmd == IPMSG_BR_ABSENCE ||
            cmd == IPMSG_NOOPERATION ? true : false;
    sprintf(buf, "%u",command);//制作格式化UDP 数据
}


//回复"我在"消息
void MainWin::MsgBrEntry(const ULONG mode,const QHostAddress &_ip)
{
    AnswerMsg(mode,_ip);
}



//收到了别人发给本机的"我在"消息
void MainWin::MsgAnsEntry()
{

}


//过滤IP
bool MainWin::FilterGetIp(const QString &_ip)
{
    //使用allAddresses命令获得所有的ip地址
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    //对找到的每一个IP地址进行比对
    foreach (QHostAddress address,list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol && address.toString() == _ip)
            return false;//如果本机所有的IPV4地址都不与传过来的这个IP地址相当，那么就说明该IP地址是一个其他主机的IPV4地址
    }
    return true;
}


//回复消息
void MainWin::AnswerMsg(const ULONG mode,const QHostAddress & _ip)
{
    //制作数据报
    char buf[20];
    MakeMsg(buf,mode);
    QByteArray datagram = QByteArray(buf);
    //发送数据报
    udpSock->writeDatagram(datagram.data(),datagram.size(),_ip,DEST_PORT);//给发来UDP广播消息的主机进行回复
}
























