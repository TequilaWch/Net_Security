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
#include <random>
#include "des.h"
#include "rsa.h"

#define SERVERPORT 4938
#define BUFFERSIZE 1024

using namespace std;

int Lsocket, Asocket;

Des des;
Rsa rsa;

string strRand(int length) {			
    char tmp;							
    string buf;						
    
    random_device rd;					
    default_random_engine random(rd());	
    
    for (int i = 0; i < length; i++) {
        tmp = random() % 52;	
        if (tmp < 26) {			
            tmp += 'a';
        } else {				
            tmp -= 26;
            tmp += 'A';
        }
        buf += tmp;
    }
    return buf;
}


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

void exchangekeyC(int sockfd, char *RemoteName)
{
    string key = strRand(8);
    cout<<"生成des密钥: "<<key<<endl;  
//  printf("生成des密钥:%s\r\n", key);
    Public_key pk;
    if(read(sockfd, &pk, sizeof(Public_key)) < 0)
    {
        perror("read");
    }
    printf("接收到公钥对(n, e): (%d, %d)\r\n", pk.n, pk.e);
    unsigned int *crypto = new unsigned int[8];
    const char* dk = key.c_str();

    for(int i = 0; i < 8; i++)
    {
        char a = dk[i];
        int b = atoi(&a);
        crypto[i] = Rsa::Encode(b, pk);
    }
    if(!send_dk(sockfd, crypto))perror("dk");
    des.setKey(key);
    safeChat(sockfd, RemoteName);

}


int main(int argc,char * [])
{
    printf("This is Client\r\nYou should configure it according to tips beneath\r\n");
    char strIpAddr[16];
    printf("Please input the server address:\r\n");
    cin>>strIpAddr;
    struct sockaddr_in sDestAddr;
    if ((Asocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket");
        exit(errno);
    }
    bzero(&sDestAddr, sizeof(sDestAddr));
    sDestAddr.sin_family = AF_INET;
    sDestAddr.sin_port = htons(SERVERPORT);
    sDestAddr.sin_addr.s_addr = inet_addr(strIpAddr);
    /* 连接服务器 */
    if (connect(Asocket, (struct sockaddr *) &sDestAddr, sizeof(sDestAddr)) != 0)
    {
        perror("Connect ");
        exit(errno);
    }
    else
    {
        printf("Connect Success!  \nBegin to chat...\n");
        exchangekeyC(Asocket, strIpAddr);
        printf("Program end\r\n");
    }
    close(Asocket);
    return 0;
}

