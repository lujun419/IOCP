#pragma once
/*
  �������Ҫ�����ڽ��ܿͻ��˵�һ�ж������Ѿ���ͻ��˷�������
*/

#include <WinSock2.h>


class ServerClient
{
private:
	char *DataBuffer; //���տͻ��˵�����
	DWORD DataSize; //DataBuffer�Ŀռ�
public:
	SOCKET ClientSock;//���տͻ��˵�socket
	ServerClient();
	~ServerClient(void);
	ServerClient(const ServerClient &Server);
	ServerClient& operator=(const ServerClient &Server);
	bool ReallocMem(const DWORD dSize);//���ٽ������ݵĿռ�
	void SetClientSock(SOCKET aClientSock);
	char* GetDataBuffer();//��ȡ��ǰ������
	DWORD GetDataSize();
	SOCKET ClearSocket(){return InterlockedExchange(reinterpret_cast<LONG *>(&ClientSock),0);}; //�����ǰclient��socket

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