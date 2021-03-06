#include "StdAfx.h"
#include "uSocketCon.h"




DWORD WINAPI AcceptThreadProc(LPVOID lpThreadParameter)
{
	/* 这里线程的处理函数,这个函数主要用户IO完成端口的并发设置数量
	*/
	uSocketCon *pSocketCon = dynamic_cast<uSocketCon *>((uSocketCon *)lpThreadParameter);
	while(TRUE)
	{   
		WaitForSingleObject(pSocketCon->GetAcceEvent(),INFINITE);
		int ActiveAcc = pSocketCon->GetActiveAcceptors();
		while(ActiveAcc < MAX_ACCEPT)
		{
			
			BOOL bRetVal = TRUE;
			SOCKET AcceptSocket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

			pIO_Operate_Data PIO =static_cast<pIO_Operate_Data>(pSocketCon->GetStackPtr()->Pop());
			/*if ()
			{
			}*/

			CreateIoCompletionPort((HANDLE)AcceptSocket,pSocketCon->GetIOCOMPort(),(DWORD)PIO,0);
			PIO->Client = new ServerClient();
			PIO->Client->SetClientSock(AcceptSocket);
			PIO->Client->ReallocMem(BUFFERSIZE);
			PIO->op_type = IO_ACCEPT;
			ZeroMemory(&PIO->overlapped,sizeof(PIO->overlapped));
			DWORD dwBytes = 0;
			if(pSocketCon->lpfnAcceptEx == NULL)
				OutputDebug(TEXT("lpfnAcceptEx is NULL."));
			bRetVal = pSocketCon->lpfnAcceptEx(pSocketCon->GetListenSocket(),
				PIO->Client->ClientSock,
				PIO->Client->GetDataBuffer(),
				4096,
				sizeof(SOCKADDR_IN) + 16,
				sizeof(SOCKADDR_IN) + 16,
				&dwBytes,
				&PIO->overlapped);

			InterlockedIncrement((LPLONG)&ActiveAcc);
			if(!bRetVal)
			{
				if(WSAGetLastError() != WSA_IO_PENDING)
				{
					OutputDebug("Create Accept socket Failed.");
					/*
					
					*/
					PIO->Client->ClearSocket();

				}
			}
		}
	}
	return 0;
}

DWORD WINAPI WorkThreadProc(LPVOID lpThreadParameter)
{
	uSocketCon *pSocketCon = (uSocketCon *)lpThreadParameter;
	DWORD TransferBytes = 0;
	DWORD CompletionKey = 0;
	LPOVERLAPPED pOverLapped = NULL;
	while (TRUE)
	{
		BOOL aret = GetQueuedCompletionStatus(pSocketCon->GetIOCOMPort(),&TransferBytes,&CompletionKey,(LPOVERLAPPED *)&pOverLapped,INFINITE);
		pIO_Operate_Data pIO_Data = CONTAINING_RECORD(pOverLapped,IO_Operate_Data,overlapped);
		std::cout<<"lianjie: "<<GetCurrentThreadId<<endl;

		switch((Operate_type)pIO_Data->op_type)
		{
		case IO_ACCEPT:
			{
				OutputDebug("Connect.acceptex");
				SOCKET aListen = pSocketCon->GetListenSocket();
				DWORD Flag = 0; 
				DWORD dwBytesRecv = 0;
				pIO_Data->op_type = IO_READ;
				WSABUF buf;
				buf.buf =pIO_Data->Client->GetDataBuffer(); 
				buf.len = pIO_Data->Client->GetDataSize();
				//setsockopt(pIO_Data->sock,SOL_SOCKET,SO_UPDATE_ACCEPT_CONTEXT,(const char *)&aListen,sizeof(aListen));
				int ret = WSARecv(pIO_Data->Client->ClientSock,&buf,1,&dwBytesRecv,&Flag,&pIO_Data->overlapped,NULL);
				OutputDebug(pIO_Data->Client->GetDataBuffer());
				break;
			}
		case IO_READ:
			{
				DWORD Flag = 0;
				DWORD dwBytesRecv = 0;
				WSABUF buf;
				
				/*int ret = WSARecv(pIO_Data->Client->ClientSock,(LPWSABUF)pIO_Data->Client->GetDataBuffer(),1,&dwBytesRecv,&Flag,&pIO_Data->overlapped,NULL);*/
				buf.buf =pIO_Data->Client->GetDataBuffer(); 
				buf.len = pIO_Data->Client->GetDataSize();
				int ret = WSARecv(pIO_Data->Client->ClientSock,&buf,1,&dwBytesRecv,&Flag,&pIO_Data->overlapped,NULL);
				OutputDebug(pIO_Data->Client->GetDataBuffer());
				if(dwBytesRecv == 0) 
				{
					//客户端断开连接
					/*delete pIO_Data->Client;
					delete pIO_Data;*/

				}
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
	FActiveAcceptors = 0;
	lpfnAcceptEx = NULL;
	lpfnGetAceeptExSockAddr = NULL;
	pStackSafe = new StacksSafe(STACKCAP);
	
	HEvent = CreateEvent(NULL,FALSE,TRUE,TEXT("AcceptexEvent"));

	WSAStartup(MAKEWORD(2,2),&WSAdata);

	IOCOMPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);

	SYSTEM_INFO info;
	GetSystemInfo(&info);

	//for(DWORD i = 0 ; i < info.dwNumberOfProcessors * 2 + 2; i++)
	//{
	//	CreateThread(NULL,0,WorkThreadProc,this,0,NULL);
	//}
	CreateThread(NULL,0,WorkThreadProc,this,0,NULL);
	LSocket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

	CreateIoCompletionPort((HANDLE)LSocket,IOCOMPort,0,0);//绑定LSocket 到IO端口上
	

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//inet_addr(ip.c_str())
	ServerAddr.sin_port = htons(port);
	if(bind(LSocket,(sockaddr *)&ServerAddr,sizeof(ServerAddr)) != 0)
	{
		char c[8]={0};
		_itoa(GetLastError(),c,10);
		OutputDebug("bind error:","123");
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
	//CreateThread(NULL,0,AcceptThreadProc,this,0,NULL);
}

uSocketCon::~uSocketCon(void)
{
	  CloseHandle(HEvent);
	  if (NULL != pStackSafe)
	  {
		  delete pStackSafe;
	  }
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