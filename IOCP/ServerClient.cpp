#include "StdAfx.h"
#include "ServerClient.h"

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
	strcpy(Server.DataBuffer,DataBuffer);
}

ServerClient::~ServerClient(void)
{
	delete[] DataBuffer;
}

bool ServerClient::ReallocMem( DWORD dSize )
{
	/*
	开辟空间需要加上 2*(sizeof(SOCKADDR_IN) + 16，见acceptex的msdn
	*/
	DataSize = dSize;
	DataBuffer = new char[DataSize + 2*(sizeof(SOCKADDR_IN) + 16)];
	return TRUE;

}

ServerClient& ServerClient::operator=( const ServerClient &Server )
{
	if(this != &Server)
	{
		char *temp = new char[Server.DataSize + 2*(sizeof(SOCKADDR_IN) + 16)];
		delete[] DataBuffer;
		strcpy(temp,Server.DataBuffer);
		DataBuffer = temp;
	}
	return *this;
}

void ServerClient::SetClientSock( SOCKET aClientSock )
{
	ClientSock = aClientSock;
}

char* ServerClient::GetDataBuffer()
{
	return DataBuffer;
}
