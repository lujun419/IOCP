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
	Operate_type op_type;//��������
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
	/*�����Ϊiocp�ķ���ˣ�iocp�Ļ����߼�������io��ɶ˿ڣ�����io��ɶ˿ڵĹ����̳߳� ������socket��Ȼ��bind������������IO�������̡߳�
	 ����IO��ɶ˿ڵĹ����̳߳أ�Ŀǰ����windows�̳߳صķ�ʽ��д����������ÿս��ĳ�socket����д��BindIO*******��ע�⣬socket�ش�������һ��Ҫ�죬��Ȼ����ɷ���������
	*/
private:
	SOCKET LSocket;
	WSADATA WSAdata;
	SOCKADDR_IN ServerAddr;
	HANDLE IOCOMPort;  //IO��ɶ˿�
	int FActiveAcceptors; //�Ѿ�������ACCEPTEX
    BOOL GetExtensions();
	HANDLE HEvent;//֪ͨ��������socket
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