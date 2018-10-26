#include "StdAfx.h"
#include "ServerClient.h"
#include "uSocketCon.h"

ServerClient::ServerClient()
{
	DataSize = 0;
	DataBuffer = NULL;
	ClientSock = 0;
}

ServerClient::ServerClient( const ServerClient &Server )
{
	//深拷贝
	DataSize = Server.DataSize;
	DataBuffer = new char[DataSize + 2*(sizeof(SOCKADDR_IN) + 16)];
	ZeroMemory(DataBuffer,DataSize + 2*(sizeof(SOCKADDR_IN) + 16));
	strcpy(Server.DataBuffer,DataBuffer);
}

ServerClient::~ServerClient(void)
{
	delete[] DataBuffer;
}

bool ServerClient::ReallocMem(const DWORD dSize )
{
	/*
	开辟空间需要加上 2*(sizeof(SOCKADDR_IN) + 16，见acceptex的msdn
	*/
	DataSize = dSize;
	DataBuffer = new char[DataSize + 2*(sizeof(SOCKADDR_IN) + 16)];
	ZeroMemory(DataBuffer,DataSize);
	return TRUE;

}

ServerClient& ServerClient::operator=( const ServerClient &Server )
{
	if(this != &Server)
	{
		char *temp = new char[Server.DataSize + 2*(sizeof(SOCKADDR_IN) + 16)];
		ZeroMemory(temp,Server.DataSize + 2*(sizeof(SOCKADDR_IN) + 16));
		delete[] DataBuffer;
		strcpy(temp,Server.DataBuffer);
		DataBuffer = temp;
	}
	return *this;
}

void ServerClient::SetClientSock( SOCKET aClientSock )
{
	//ClientSock = aClientSock;
	InterlockedExchange(reinterpret_cast<LONG *>(&ClientSock),aClientSock);
}

char* ServerClient::GetDataBuffer()
{
	return DataBuffer;
}

DWORD ServerClient::GetDataSize()
{
	return DataSize;
}

void * StacksSafe::Pop()
{
	pIO_Operate_Data pData = NULL;
	pData = (pIO_Operate_Data)InterlockedPopEntrySList(pSlist);
	if (NULL == pData)
	{
		pData = (pIO_Operate_Data)_aligned_malloc(sizeof(IO_Operate_Data),MEMORY_ALLOCATION_ALIGNMENT);
	}
	if (NULL != pData)
	{
		//ZeroMemory(pData,sizeof(IO_Operate_Data));
	}
	return pData;
}

StacksSafe::StacksSafe( DWORD Capacity )
{
	if ((Capacity > 0) && (Capacity <= STACKMAX))
	{
		this->Capacity = Capacity;
	}
	else
		OutputDebug("Set Capacity Error.");
	
	pSlist = (PSLIST_HEADER)_aligned_malloc(sizeof(SLIST_HEADER),MEMORY_ALLOCATION_ALIGNMENT);
	InitializeSListHead(pSlist);
}

void StacksSafe::push( void * data )
{
	if (NULL == data)
	{
		return;
	}
	if (this->Capacity>QueryDepthSList(pSlist))
	{
		InterlockedPushEntrySList(pSlist,(PSINGLE_LIST_ENTRY)data);
	}
    _aligned_free(data);
}

StacksSafe::~StacksSafe()
{
  if (QueryDepthSList(pSlist) > 0)
  {
	  
  }
  InterlockedFlushSList(pSlist);
  void *pData = InterlockedPopEntrySList(pSlist);
  if (NULL != pData)
  {
	  OutputDebug("链表非空.");
  }
}
