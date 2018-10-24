//#pragma once
#ifndef SAVEDATA_H
#define SAVEDATA_H
#include <Record.h>
#include <iostream>


class SaveData
{
private:
	NodeData *head;
	NodeData *tail;
public:
	SaveData(void);
	~SaveData(void);
	NodeData* MakeData(int data);
	//传入的N的下标表示从1开始
	void Insert(NodeData *Data,int n);
	void Push(NodeData *Data);
	void Pop(NodeData *Data);
	void Display();
};
#endif
