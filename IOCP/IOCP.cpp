// IOCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SaveData.h"
//#include <WinSock2.h>
#include<iostream>
#include "uSocketCon.h"
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	//cout<<sizeof(SOCKADDR_IN)<<endl;
	//char a[5]={'a','b','c','d'};
	//char (*p)[5] = &a;
	//char (*p1)[5] =(char (*)[5])a;
	//p+=2;
	//p++;
	//cout<<sizeof(p)<<endl;
	//cout<<**(p) <<" "<<**p1<<endl;


	//SaveData *PSave = new SaveData();
	//for(int i = 0;i < 10; i++)
	//{
	//	PSave->Push(PSave->MakeData(i));
	//	//cout<<i<<endl;
	//}
	//PSave->Insert(PSave->MakeData(100),2);
	////psave->display();
	//PSave->Pop(PSave->MakeData(7));
	//PSave->Display();
	//system("pause");
	//delete PSave;	
	uSocketCon *pSocketCon = new uSocketCon("192.168.9.211",8000);
	system("pause");

	return 0;
}

