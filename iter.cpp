#include "iter.h"
#include<string>



Iter::Iter()
{
//    addConThread = new MyThread;//创建主动连接线程
//    tcpAdder = new Client;//创建主动添加TCP连接的对象
//    tcpAdder->moveToThread(addConThread);//把这个对象移动到子线程中
//    QObject::connect(this, SIGNAL(sigAddTcpThread()), tcpAdder, SLOT(addNewTcp())); // 这里的slot()函数，相当于run()函数的作用
//    QObject::connect(this,SIGNAL(addTcp(QString)),tcpAdder,SLOT(slot(QString)));//cad命令时,执行槽tcpAdder 的slot槽
//    addConThread->start();
//    emit sigAddTcpThread();//启动主动添加TCP的线程 addConThread

//    rwLock.lockForWrite();
//    qDebug()<<QThread::currentThreadId()<<"Inter constructor!";
//    rwLock.unlock();


}

Iter::~Iter()
{


   if(addConThread){
       delete addConThread;
       addConThread = NULL;
   }
}


void Iter::interacter()
{
   // int iCurNum = 0;
   // int sockNum = 100;
    //QTcpSocket m_pSocket[100];//创建100个套接字;

    //std::map<std::string,int> myIpMap;
//      rwLock.lockForWrite();
//      qDebug()<<QThread::currentThreadId()<<"Inter";
//      rwLock.unlock();


    char com[20], *fileName;
    int count;

    while(1)
    {
        rwLock.tryLockForRead();
       // printf("\n(AutoSend):");
        std::cout<<"(AutoSend):";
        rwLock.unlock();


        fgets(com, sizeof(com), stdin);//
        transfStr(com, 1); //去掉开头结尾空白字符，变成全小写

        if (!strcmp(com, "list") ||
                !strcmp(com, "ls"))
        {
                  //显示所有的TCP连接
                  if(Server::getInstance()->showIpList()){
                    }
                  else{

                      std::cout<<"No connecting ip!"<<std::endl;

                  }
        }
        else if (!strcmp(com, "quit") ||
                 !strcmp(com, "q"))
        {
        QCoreApplication::exit(0);//退出进程
        }
        else if (!strcmp(com, "refresh") ||
                 !strcmp(com, "rf"))
        {

        }
        else if (!strcmp(com, "talk") ||
                 !strcmp(com, "tk"))
        {


        }
        else if (!strcmp(com, "sendfile") ||
                 !strcmp(com, "sf"))
        {

            std::string chatContext;//会话内容
            std::string chatIpAddr;//会话地址
            std::cout<<"IP:";
            std::getline(std::cin,chatIpAddr);//获得要对话的IP地址
            std::cout<<":";
            std::getline(std::cin,chatContext);//获得要对话的内容
            QString qstrIp = QString::fromStdString(chatIpAddr);//把要对话的IP地址转换成QString格式
            QString qstrContext = QString::fromStdString(chatContext);//把要对话的IP地址转换成QString格式
            emit ipChat(qstrIp,qstrContext);


        }
        else if (!strcmp(com, "getfile") ||
                 !strcmp(com, "gf"))
        {

        }
        else if (!strcmp(com, "ceaseSend") ||
                 !strcmp(com, "cs"))
        {

        }

        else if (!strcmp(com, "cad") ||
                 !strcmp(com, "c"))
        {

          std::cout<<"IP:";
          std::string ipAddr;
          std::getline(std::cin,ipAddr);
          QString qstrip = QString::fromStdString(ipAddr);
          emit addTcp(qstrip);//发送给主体IP地址

        }
        else if (!strcmp(com, "help") ||
                 !strcmp(com, "h"))
        {
            printf(IMHELP);
        }


        else if (!strcmp(com, "clear") ||
                 !strcmp(com, "clr"))
        {
            system("cls");
        }

        else if (!strcmp(com, "dir") ||
                 !strcmp(com, "DIR"))
        {
            system("dir");
        }

        else
        {
            std::cout<<com<<": command not found"<<std::endl;


        }

    }

}







void Iter::transfStr(char *dest, int flag)
{
    char *ptr;
    int len;
    ptr = dest;
    while (isspace(*ptr))
        ptr++;
    len = strlen(ptr);
    if (ptr > dest)
        memmove(dest, ptr, len+1);
    ptr = dest+len-1;
    while (isspace(*ptr))
        ptr--;
    *(ptr+1) = '\0';
    ptr = dest;
    if (flag == 1)
        while (*ptr!='\0'){
            *ptr = tolower(*ptr);
            ptr++;
        }
}




bool Iter::isFindSuc(std::map<std::string, int> map, std::string key)
{

    std::map<std::string,int>::iterator it = map.find(key);//看看map中是否有这个地址
    if(it==map.end()){
        return false;//没有找到
    }
    else{
        return true;
    }

}






