#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "md5.h"
#include<iostream>
#include<string.h>
using namespace std;
char* store1 = new char[40];
char* store2 = new char[40];
void md5_ori(char *message, bool store)
{
	MD5 md5;
	MD5Init(&md5);
	int i;
	// unsigned char encrypt[100];
	// printf("Input the message:  ");

	// scanf("%s", &encrypt);
	unsigned char decrypt[16];
	unsigned char *encrypt = (unsigned char*)message;
	MD5Update(&md5, encrypt, strlen((char *)encrypt));
	MD5Final(&md5, decrypt);
	printf("md5 code of \"%s\" is: ",encrypt); 
	char hexstr[33];
	for (i = 0; i < 16; i++)
	{
		sprintf(&hexstr[i * 2], "%02x", decrypt[i]);
	}
	for (int i = 0; i < 31; i += 2)
	{
		cout << hexstr[i] << hexstr[i + 1];
	}
	if(store)
	{	
		for(int i=0;i<strlen(hexstr);i++)
		{
			store1[i] = hexstr[i];
		}
		delete[]encrypt;
	}
	printf("\n");
	
}
void compare(char* a,char* b)
{
	int lenb = strlen(b);
	for(int i=0;i<lenb-1;i++)
	{
		if(a[i]!=b[i]){cout<<"不一致"<<endl;return;}
	}
	cout<<"比对一致"<<endl;return;

}
void md5_t()
{   
	md5_ori("",false);
	md5_ori("a",false);
	md5_ori("abc",false);
	md5_ori("message digest",false);
	md5_ori("abcdefghijklmnopqrstuvwxyz",false);
	md5_ori("ABCDEFGHIJKLMNOPQRSTUVWXYZ",false);
	md5_ori("12345678901234567890123456789012345678901234567890123456789012345678901234567890",false);
	cout<<endl<<"你可以在MD5官方文档验证这些输出结果，他们都是正确的"<<endl;
	return;
}
void md5_c(char *filepath)//md5.exe -c txtfile :打印file1的MD5
{	
	printf("file: %s\n",filepath);
	FILE*fp;  
    fp=fopen(filepath,"rb");        
	fseek(fp,0L,SEEK_END); 
	int flen=ftell(fp);
	char *buf = new char[flen+1];
	fseek(fp,0L,SEEK_SET);
	fread(buf,flen,1,fp);
	md5_ori(buf,false);
	return;
}
void md5_v(char *filepath)//md5.exe -v txtfile :输入MD5并与txtfile的MD5比较
{
	char* md5 = new char[40];
	cout<<"请输入要用于比对的md5: ";
	cin>>md5;
	printf("file: %s\n",filepath);
	bool store = true;
	FILE* fp;  
    fp=fopen(filepath,"rb");        
	fseek(fp,0L,SEEK_END); 
	int flen=ftell(fp);
	char *buf = new char[flen+1];
	fseek(fp,0L,SEEK_SET);
	fread(buf,flen,1,fp);
	md5_ori(buf,store);
	compare(store1,md5);
	return;
}
void md5_V(char *filepath,char *mdpath)//md5.exe -V txtfile mdfile:读取mdfile的内容与file1的MD5比较
{	
	printf("file: %s\n",filepath);
	bool store = true;
	FILE* fp;  
    fp=fopen(filepath,"rb");        
	fseek(fp,0L,SEEK_END); 
	int flen=ftell(fp);
	char *buf = new char[flen+1];
	fseek(fp,0L,SEEK_SET);
	fread(buf,flen,1,fp);
	fclose(fp);
	md5_ori(buf,store);
	printf("mdfile: %s\n",mdpath);
	FILE* mdfp;  
    mdfp=fopen(mdpath,"rb");        
	fseek(mdfp,0L,SEEK_END); 
	int mdflen=ftell(mdfp);
	char *mdbuf = new char[mdflen+1];
	fseek(mdfp,0L,SEEK_SET);
	fread(mdbuf,mdflen,1,mdfp);
	fclose(mdfp);
	compare(store1,mdbuf);

	return;
}
void md5_f(char *filepath,char *filepath2)//md5.exe -f txtfile1 txtfile2 :打印并比较file1和file2的MD5
{
	printf("file1: %s\n",filepath);
	bool store = true;
	FILE* fp1;  
    fp1=fopen(filepath,"rb");        
	fseek(fp1,0L,SEEK_END); 
	int flen1=ftell(fp1);
	char *buf1 = new char[flen1+1];
	fseek(fp1,0L,SEEK_SET);
	fread(buf1,flen1,1,fp1);
	fclose(fp1);
	md5_ori(buf1,store);
	for(int i = 0;i<strlen(store1);i++)
	{
		store2[i] = store1[i];
	}
	memset(store1,0,40);
	printf("file2: %s\n",filepath);
	FILE* fp2;  
    fp2=fopen(filepath2,"rb");        
	fseek(fp2,0L,SEEK_END); 
	int flen2=ftell(fp2);
	char *buf2 = new char[flen2+1];
	fseek(fp2,0L,SEEK_SET);
	fread(buf2,flen2,1,fp2);
	fclose(fp2);
	md5_ori(buf2,store);

	compare(store1,store2);
	return;
}
int main(int argc, char *argv[]) 
{

	if (argc < 2)
	{
		cout<<"指令错误，请使用 ./md5.exe -h 以获取帮助:\n";
		return -1;
	}
	else
	{
		string op = argv[1];

		if(op == "-t")
		{
			md5_t();
		}
		if(op == "-h")
		{
			cout<<"md5.exe使用方法"<<endl
			<<"./md5.exe [选项] [文件1路径] [文件2路径]"<<endl
			<<"./md5.exe -h :查看帮助"<<endl
			<<"./md5.exe -t :查看测试信息"<<endl
			<<"./md5.exe -c txtfile :打印file1的MD5"<<endl
			<<"./md5.exe -v txtfile :输入MD5并与txtfile的MD5比较"<<endl
			<<"./md5.exe -V txtfile mdfile:读取mdfile的内容与file1的MD5比较"<<endl
			<<"./md5.exe -f txtfile1 txtfile2 :打印并比较file1和file2的MD5"<<endl
			<<endl<<endl<<"注意，所有涉及到的md5值需要以16进制出现且不包含0x前缀。文件中任何额外的换行符等内容也会导致提示不一致"<<endl;
			
		}
		if(op == "-c")
		{
			md5_c(argv[2]);
		}
		if(op == "-v")
		{
			md5_v(argv[2]);
		}
		if(op == "-V")
		{
			md5_V(argv[2],argv[3]);
		}
		if(op == "-f")
		{
			md5_f(argv[2],argv[3]);
		}

	}

	return 0;
}
