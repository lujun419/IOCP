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
	bool ReallocMem(DWORD dSize);//���ٽ������ݵĿռ�
	void SetClientSock(SOCKET aClientSock);
	char* GetDataBuffer();//��ȡ��ǰ������


};