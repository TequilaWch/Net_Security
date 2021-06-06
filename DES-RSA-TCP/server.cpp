// File: des.h
// Auth: WCH
// Date: 2021/3/14
// Syst: Linux(Ubuntu18.04)
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <net/if.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <memory.h>
#include <sys/ioctl.h>
#include "des.h"
#include "rsa.h"

#define SERVERPORT 4938
#define BUFFERSIZE 1024

using namespace std;

int Lsocket, Asocket;

Des des;
Rsa rsa;

bool send_data(int sockfd,string data) //发送数据
{
    const char* cmd = data.c_str();
    size_t size  = strlen(cmd)*sizeof(char);
    size_t total = 0;
    while(total < size)
    {
        size_t wr = write(sockfd, cmd, size);
        if(wr < 0)
            perror("write error");
        else
            total += wr;
    }
   if(total == size)
        return true; 
    else
        return false;
}
bool send_pk(int sockfd, Public_key pk)   //发送RSA公钥
{
    size_t size  = sizeof(Public_key);
    size_t total = 0;
    while(total < size)
    {
        size_t wr = write(sockfd,&pk, size);
        if(wr < 0)
            perror("write error");
        else
            total += wr;
    }
   if(total == size)
        return true; 
    else
        return false;
}
bool send_dk(int sockfd,unsigned int* dk)  // 发送加密后的DES密钥
{
    size_t size  = sizeof(unsigned int) * 8;
    size_t total = 0;
    while(total < size)
    {
        size_t wr = write(sockfd,dk,size);
        if(wr < 0)
            perror("write error");
        else
            total += wr;
    }
   if(total == size)
        return true; 
    else
        return false;
}

void safeChat(int Sock,char *RemoteName)
{
    char recvbuf[BUFFERSIZE];
    memset(recvbuf,0,sizeof(recvbuf));
    
    string recv,send;
    
    pid_t nPid;
    nPid = fork();
    if(nPid != 0)                     
    {
        while(1)
        {
            memset (recvbuf,0,sizeof(recvbuf));
            //接收客户端发送来的内容
            if(read(Sock,recvbuf,sizeof(recvbuf))<0)
            {
                perror("read error");
            }
            string s = recvbuf;
            
            string decode_data = des.decode(s);
            if(decode_data[0] == 'q'&&decode_data[1] == 'u'&&decode_data[2] == 'i'&&decode_data[3] == 't')exit(0);
            cout<<"Receive message form "<<RemoteName<<" : "<<decode_data<<'\n';       
        }
    }
    else                                                                                                               //父线程发送消息
    {
        while(1)
        {
            string reply;
            getline(cin,reply);
            string encode_reply = des.encode(reply);

            if(!send_data(Sock, encode_reply))
            {
                perror("send");
                exit(1);
            }
            if(reply[0] == 'q'&&reply[1] == 'u'&&reply[2] == 'i'&&reply[3] == 't')exit(0);
            
        }
    }
}

void exchangekeyS(int sockfd, char *RemoteName)
{

    char buf[BUFFERSIZE];

    memset(buf, 0, sizeof(buf));
 
    string s = buf;
    if(s[0] == 'q' && s[1] == 'u' && s[2] == 'i' && s[3] == 't')exit(0);

    rsa.init();

    Public_key pk = rsa.get_pk();
    Secret_key sk = rsa.get_sk();
    rsa.check_key();
    
    if(!send_pk(sockfd, pk))
    {
        perror("pk");
    }

    printf("已发送公钥\r\n");

    unsigned int* dk = new unsigned int[8];
    while(true)
    {
	if(read(sockfd, dk, sizeof(unsigned int) * 8) < 0) perror("read dk");
        else break;
    }
    
    string key = "";
    for(int i = 0; i < 8; i++)
    {
        key += to_string(Rsa::Decode(dk[i], sk));
    }
    des.setKey(key);
    cout<<"已接受des密钥,开始通信"<<endl; 
    safeChat(sockfd, RemoteName);

}


int main()
{
    printf("This is Server\r\nNow Server begin to work\r\n");

    
    socklen_t nLength = 0;
    struct sockaddr_in sLocalAddr, sRemoteAddr;
    if ((Lsocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    bzero(&sLocalAddr, sizeof(sLocalAddr));
    sLocalAddr.sin_family = PF_INET;
    sLocalAddr.sin_port = htons(SERVERPORT);
    sLocalAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(Lsocket, (struct sockaddr *) &sLocalAddr, sizeof(struct sockaddr))== -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(Lsocket, 5) == -1)
    {
        perror("listen");
        exit(1);
    }
    printf("Listening...\n");
    nLength = sizeof(struct sockaddr);
    if ((Asocket = accept(Lsocket, (struct sockaddr *) &sRemoteAddr,&nLength)) == -1)
    {
        perror("accept");
        exit(errno);
    }
    else
    {
        close(Lsocket);
        printf("server: got connection from %s, port %d, socket %d\n",inet_ntoa(sRemoteAddr.sin_addr),ntohs(sRemoteAddr.sin_port), Asocket);
        exchangekeyS(Asocket, inet_ntoa(sRemoteAddr.sin_addr));

        printf("Program end\r\n");
        close(Asocket);
    }
    return 0;
}


