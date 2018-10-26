#pragma once
//#ifndef USOCKETCON_H
//#define USOCKETCON_H

#pragma comment(lib,"Ws2_32.lib")


#include <iostream>
#include <WinSock2.h>
#include "uPublicFunc.h"
#include <MSWSock.h>
#include <string>

#include <Windows.h>
#include "Record.h"
#include "ServerClient.h"



enum Operate_type{IO_ACCEPT,IO_READ,IO_WRITE};

typedef struct _IO_Operate_Data_{
	OVERLAPPED overlapped;
	Operate_type op_type;//操作类型
	ServerClient *Client;
	_IO_Operate_Data_(){Client = NULL;};
	
}IO_Operate_Data,*pIO_Operate_Data;
//#include <WinSock.h>
using namespace std;

//----
//const DWORD BUFFERSIZE= 0x1000;
#define BUFFERSIZE 0x1000
#define  MAX_ACCEPT  1000
#define  STACKMAX 65535
#define STACKCAP 1024*8
//----

const int Data_Buffer_Size = 1024 * 4;




class uSocketCon
{
	/*这个作为iocp的服务端，iocp的基本逻辑：创建io完成端口，创建io完成端口的工作线程池 ，创建socket，然后bind，监听，创建IO的链接线程。
	 这里IO完成端口的工作线程池，目前采用windows线程池的方式编写，后期如果用空将改成socket池来写（BindIO*******，注意，socket池处理任务一定要快，不然会造成服务器卡）
	*/
private:
	SOCKET LSocket;
	WSADATA WSAdata;
	SOCKADDR_IN ServerAddr;
	HANDLE IOCOMPort;  //IO完成端口
	int FActiveAcceptors; //已经创建的ACCEPTEX
    BOOL GetExtensions();
	HANDLE HEvent;//通知创建接受socket
	StacksSafe *pStackSafe;
public:
	
	uSocketCon(string ip,int port);
	~uSocketCon(void);
	LPFN_ACCEPTEX lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS lpfnGetAceeptExSockAddr;
	HANDLE GetIOCOMPort();
	SOCKET GetListenSocket();
	StacksSafe *GetStackPtr(){return pStackSafe;};
	int GetActiveAcceptors(){return FActiveAcceptors;};
	HANDLE GetAcceEvent(){return HEvent;};
	
};



//#endif