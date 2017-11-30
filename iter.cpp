#include "iter.h"
#include<string>

Iter::Iter()
{



}

Iter::~Iter()
{


   if(addConThread){
       delete addConThread;
       addConThread = NULL;
   }
}


void Iter::Interacter()
{
    char com[20], *fileName;
    int count;

    while(1)
    {
        rwLock.tryLockForRead();
        std::cout<<"(AutoSend):";
        rwLock.unlock();

        fgets(com, sizeof(com), stdin);//
        TransfStr(com, 1); //去掉开头结尾空白字符，变成全小写

        if (!strcmp(com, "list") ||
                !strcmp(com, "ls"))
        {
                  //显示所有的TCP连接
                  if(MainWin::GetInstance()->ShowIpList()){
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
            emit SendFile(qstrIp,qstrContext);


        }
        else if (!strcmp(com, "getfile") ||
                 !strcmp(com, "gf"))
        {

        }
        else if (!strcmp(com, "cat"))

        {

        }


        else if (!strcmp(com, "cad") ||
                 !strcmp(com, "c"))
        {
          std::cout<<"IP:";
          std::string ipAddr;
          std::getline(std::cin,ipAddr);//获得IP地址
          QString qstrip = QString::fromStdString(ipAddr);
          emit AddTcp(qstrip);//发送给主体IP地址

        }
        else if (!strcmp(com, "help") ||
                 !strcmp(com, "h"))
        {
            printf(IMHELP);
        }

        else if (!strcmp(com, "version") ||
                 !strcmp(com, "v"))
        {
            std::cout<<VERSION<<std::endl;
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



void Iter::TransfStr(char *dest, int flag)
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


bool Iter::IsFindSuc(std::map<std::string, int> map, std::string key)
{

    std::map<std::string,int>::iterator it = map.find(key);//看看map中是否有这个地址
    if(it==map.end()){
        return false;//没有找到
    }
    else{
        return true;
    }

}






