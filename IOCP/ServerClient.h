#pragma once
/*
  这个类主要是用于接受客户端的一切东西，已经向客户端发送数据
*/

#include <WinSock2.h>


class ServerClient
{
private:
	char *DataBuffer; //接收客户端的数据
	DWORD DataSize; //DataBuffer的空间
public:
	SOCKET ClientSock;//接收客户端的socket
	ServerClient();
	~ServerClient(void);
	ServerClient(const ServerClient &Server);
	ServerClient& operator=(const ServerClient &Server);
	bool ReallocMem(const DWORD dSize);//开辟接收数据的空间
	void SetClientSock(SOCKET aClientSock);
	char* GetDataBuffer();//获取当前的数据
	DWORD GetDataSize();
	SOCKET ClearSocket(){return InterlockedExchange(reinterpret_cast<LONG *>(&ClientSock),0);}; //清除当前client的socket

};

class StacksSafe
{
private:
	PSLIST_HEADER pSlist; 
	DWORD Capacity;
public:
	StacksSafe(DWORD Capacity);
	~StacksSafe();
	void * Pop();
	void push(void * data);
		//InitializeSListHead()
};