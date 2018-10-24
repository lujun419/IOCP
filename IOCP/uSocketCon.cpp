#include "StdAfx.h"
#include "uSocketCon.h"




DWORD WINAPI AcceptThreadProc(LPVOID lpThreadParameter)
{
	/* 这里线程的处理函数,这个函数主要用户IO完成端口的工作线程
	*/
	uSocketCon *pSocketCon = (uSocketCon *)lpThreadParameter;
	BOOL bRetVal = TRUE;
	SOCKET AcceptSocket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	
	pIO_Operate_Data PIO= new IO_Operate_Data;
	CreateIoCompletionPort((HANDLE)AcceptSocket,pSocketCon->GetIOCOMPort(),(DWORD)PIO,0);

	
	PIO->sock = AcceptSocket;
	PIO->databuf.buf = PIO->buffer;
	PIO->databuf.len = sizeof(PIO->buffer);
	PIO->type = IO_ACCEPT;
    ZeroMemory(&PIO->overlapped,sizeof(PIO->overlapped));
	DWORD dwBytes = 0;
    if(pSocketCon->lpfnAcceptEx == NULL)
		OutputDebug(TEXT("lpfnAcceptEx is NULL."));
	bRetVal = pSocketCon->lpfnAcceptEx(pSocketCon->GetListenSocket(),
										PIO->sock,
										&PIO->databuf,
										0,
										sizeof(SOCKADDR_IN) + 16,
										sizeof(SOCKADDR_IN) + 16,
										&dwBytes,
										&PIO->overlapped);
	
	if(bRetVal)
		return 0;
	else
	{
		if(WSAGetLastError() == WSA_IO_PENDING)
			return 0;
		else
			return -1;
	}
}

DWORD WINAPI WorkThreadProc(LPVOID lpThreadParameter)
{
	uSocketCon *pSocketCon = (uSocketCon *)lpThreadParameter;
	DWORD TransferBytes = 0;
	DWORD CompletionKey = 0;
	LPOVERLAPPED pOverLapped = NULL;
	while (TRUE)
	{
		GetQueuedCompletionStatus(pSocketCon->GetIOCOMPort(),&TransferBytes,&CompletionKey,(LPOVERLAPPED *)&pOverLapped,INFINITE);
		pIO_Operate_Data pIO_Data = CONTAINING_RECORD(pOverLapped,IO_Operate_Data,overlapped);
		std::cout<<"lianjie"<<endl;

		switch((Operate_type)pIO_Data->type)
		{
		case IO_ACCEPT:
			{
				OutputDebug("Connect.acceptex");
				SOCKET aListen = pSocketCon->GetListenSocket();
				DWORD Flag = 0;
				DWORD dwBytesRecv = 0;
				setsockopt(pIO_Data->sock,SOL_SOCKET,SO_UPDATE_ACCEPT_CONTEXT,(const char *)&aListen,sizeof(aListen));
				WSARecv(pIO_Data->sock,&pIO_Data->databuf,1,&dwBytesRecv,&Flag,&pIO_Data->overlapped,NULL);

				break;
			}
		case IO_READ:
			{
				OutputDebug("IO_READ");
				break;
			}
		case IO_WRITE:
			{
				OutputDebug("IO_WRITE");
				break;
			}
		}
	}

	return 0;
}

uSocketCon::uSocketCon(string ip,int port)
{
	lpfnAcceptEx = NULL;
	lpfnGetAceeptExSockAddr = NULL;
	WSAStartup(MAKEWORD(2,2),&WSAdata);

	IOCOMPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);

	SYSTEM_INFO info;
	GetSystemInfo(&info);

	for(DWORD i = 0 ; i < info.dwNumberOfProcessors * 2 + 2; i++)
	{
		CreateThread(NULL,0,WorkThreadProc,this,0,NULL);
	}

	LSocket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

	CreateIoCompletionPort((HANDLE)LSocket,IOCOMPort,0,0);//绑定LSocket 到IO端口上
	

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//inet_addr(ip.c_str())
	ServerAddr.sin_port = htons(port);
	if(bind(LSocket,(sockaddr *)&ServerAddr,sizeof(ServerAddr)) != 0)
	{
		char c[8]={0};
		_itoa(GetLastError(),c,10);
		OutputDebug("bind error:",c);
		WSACleanup();
		return;
	};
	listen(LSocket,SOMAXCONN);

	if(!GetExtensions())//获取扩展函数
	{
	  WSACleanup();
	  return;
	};


		


	CreateThread(NULL,0,AcceptThreadProc,this,0,NULL);
	CreateThread(NULL,0,AcceptThreadProc,this,0,NULL);
}

uSocketCon::~uSocketCon(void)
{
}

BOOL uSocketCon::GetExtensions()
{
	BOOL flag =  TRUE;
	DWORD dwbytes = 0;
	GUID FnGuid = WSAID_ACCEPTEX;
	if(0 != WSAIoctl(LSocket,
		             SIO_GET_EXTENSION_FUNCTION_POINTER,
					 &FnGuid, sizeof(FnGuid),
					 &lpfnAcceptEx,sizeof(lpfnAcceptEx),
					 &dwbytes,
					 NULL,
					 NULL))
	{
		flag = FALSE;
		lpfnAcceptEx = NULL;
		char p[8] = {0};
		_itoa_s(WSAGetLastError(),p,10);
		OutputDebug(TEXT("Load accepteex:"),p);
	};

	if(flag)
	{
		 GUID FnGuidSocketAddr = WSAID_GETACCEPTEXSOCKADDRS;
		 if(0 != WSAIoctl(LSocket,
							SIO_GET_EXTENSION_FUNCTION_POINTER,
							&FnGuidSocketAddr,sizeof(FnGuidSocketAddr),
							&lpfnGetAceeptExSockAddr,sizeof(lpfnGetAceeptExSockAddr),
							&dwbytes,
							NULL,
							NULL))
		{
			flag = FALSE;
			lpfnGetAceeptExSockAddr = NULL;
		 };
	};
	return flag;
}

HANDLE uSocketCon::GetIOCOMPort()
{
	return IOCOMPort;
}
SOCKET uSocketCon::GetListenSocket()
{
	return LSocket;
}