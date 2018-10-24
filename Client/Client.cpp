// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib,"Ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <MSWSock.h>
#include <string>

#include "uPublicFunc.h"
//#include <WinSock.h>
using namespace std;

const int Data_Buffer_Size = 1024 * 4;
enum Operate_type{IO_ACCEPT,IO_READ,IO_WRITE};

typedef struct _IO_Operate_Data_{

	OVERLAPPED overlapped;
	WSABUF databuf;
	char buffer[Data_Buffer_Size]; //内容
	Operate_type type;//操作类型
	SOCKET sock;//操作套接字
	DWORD len;  //接受内容的长度
}IO_Operate_Data,*pIO_Operate_Data;

int _tmain(int argc, _TCHAR* argv[])
{
	/*WSADATA WSAdata;
	WSAStartup(MAKEWORD(2,2),&WSAdata);

	SOCKET cSocket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("192.168.9.211");//inet_addr(ip.c_str())
	ServerAddr.sin_port = htons(8000);
	if(bind(cSocket,(sockaddr *)&ServerAddr,sizeof(ServerAddr)) != 0)
	{
		char c[8]={0};
		_itoa(GetLastError(),c,10);
		OutputDebug("bind error:",c);
		WSACleanup();
		return 0;
	};
	GUID FnGuidSocketAddr = WSAID_CONNECTEX;
	LPFN_CONNECTEX lpfnConnEx = NULL;
	DWORD dwbytes = 0;
	char *data = "asdfasd";
	DWORD dwSend = 0;
	OVERLAPPED ol;
	ZeroMemory(&ol,sizeof(ol));

	if(0!= WSAIoctl(cSocket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&FnGuidSocketAddr,sizeof(FnGuidSocketAddr),
		&lpfnConnEx,sizeof(lpfnConnEx),
		&dwbytes,
		NULL,
		NULL))
	{

	}
	else
	{
		if (!lpfnConnEx(cSocket,
			    (sockaddr*)&ServerAddr,
				sizeof(SOCKADDR_IN),
				NULL,
				0,
				&dwSend,
				&ol))
		{
			char c[8] = {0};
			_itoa(GetLastError(),c,10);
			OutputDebug("ConnectEx error:",c);
		}
	}
	WSABUF buf;
	buf.buf = data;
	buf.len = strlen(data);
	dwbytes = 0;
    DWORD dwFlag = 0;
	//ZeroMemory(&ol,sizeof(OVERLAPPED));

	if(!WSASend(cSocket,&buf,1,&dwbytes,dwFlag,&ol,NULL))
	{
		char c[8] = {0};
		_itoa(GetLastError(),c,10);
		OutputDebug("WSASend error:",c);
	}
	dwbytes = 0;
    if(!WSAGetOverlappedResult(cSocket,&ol,&dwbytes,TRUE,&dwFlag))
	{
		cout<<"等待一步結果失敗."<<endl;
	}
	system("pause");
	WSACleanup();*/

	//WSADATA WSAdata;
	//WSAStartup(MAKEWORD(2,2),&WSAdata);
	//SOCKET cSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	//if (cSocket == 0)
	//{
	//	std::cout<<"socket error."<<endl;
	//	return 0;
	//}
	//sockaddr_in ServerAddr; 
	//ServerAddr.sin_family = AF_INET;
	//ServerAddr.sin_addr.S_un.S_addr = inet_addr("192.168.9.211");
	//ServerAddr.sin_port = htons(8000);

	//DWORD ret = connect(cSocket,(struct sockaddr*)&ServerAddr,sizeof(SOCKADDR_IN));
	//if(ret == 0)
	//{
	//	std::cout<<"connect success."<<endl;
	//}
	//else
	//{
	//	std::cout<<GetLastError()<<endl;
	//}
	//char sndbuffer[80] = {0};
	//strcpy(sndbuffer,TEXT("hello Server!"));
	//ret = send(cSocket,sndbuffer,strlen(sndbuffer),0);
	//char data = "hello.\n";
	//WSABUF buf;
	//buf.buf = data;
	//buf.len = strlen(data);
	//if(ret>0)
	//{
	//	std::cout<<"send context:"<<sndbuffer<<endl;
	//}
 //   system("pause");
	//closesocket(cSocket);

WSADATA WSAdata;
WSAStartup(MAKEWORD(2,2),&WSAdata);
SOCKET cSocket = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_IP,0,0,WSA_FLAG_OVERLAPPED);
if (cSocket == 0)
{
	std::cout<<"socket error."<<endl;
	return 0;
}
sockaddr_in ServerAddr; 
ServerAddr.sin_family = AF_INET;
ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
ServerAddr.sin_port = htons(8000);

DWORD ret = connect(cSocket,(struct sockaddr*)&ServerAddr,sizeof(SOCKADDR_IN));
if(ret == 0)
{
	std::cout<<"connect success."<<endl;
}
else
{
	std::cout<<GetLastError()<<endl;
}
char sndbuffer[80] = {0};
strcpy(sndbuffer,TEXT("hello Server!"));
//ret = send(cSocket,sndbuffer,strlen(sndbuffer),0);
char *data = "hello.\n";
WSABUF buf;
buf.buf = data;
buf.len = strlen(data);
DWORD dwBytes = 0;
OVERLAPPED aol;
ZeroMemory(&aol,sizeof(aol));
for (int i = 0; i < 1; i++)
{
	ret = WSASend(cSocket,&buf,1,&dwBytes,0,&aol,NULL);
	if(ret == 0)
	{
		std::cout<<dwBytes<<endl;
		std::cout<<"send context:"<<sndbuffer<<endl;
	}
};
system("pause");
closesocket(cSocket);
	return 0;
}

