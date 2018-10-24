#include "StdAfx.h"
#include "SaveData.h"

SaveData::SaveData(void)
{
	head = NULL;
	tail = NULL;
	/*head->next = NULL;
	head->pre = NULL;
	//
	tail->next = NULL;
	tail->pre = NULL;*/
}

SaveData::~SaveData(void)
{
}


NodeData* SaveData::MakeData(int data)
{ 
	NodeData *Data = new NodeData;
	Data->Data = data;
	Data->next = NULL;
	Data->pre = NULL;
	return Data;
}

void SaveData::Insert(NodeData *Data,int n){
	
	NodeData *temp = head;
	int i = 0; 
	while(NULL != temp)
	{
		if( i == n -1)
		{
			NodeData * temp1 = temp ->pre;
			temp1->next = Data;
			Data->next = temp;
			temp->pre = Data;
			Data->pre = temp1;

			break;
		}
		temp = head->next;
		i++;
	}
}
void SaveData::Push(NodeData *Data){
	


	/*tail->next = Data;
	tail->pre = NULL;
	Data->pre = tail;*/
	

	if(head == NULL)
	{
		head = Data;
		tail = head;
	}
	else
	{
	  Data->pre = tail;
	  tail->next = Data;
	  tail = Data;//指向的尾节点
	}
		
	
}

void SaveData::Pop(NodeData *Data){

	if(NULL != Data)
	{
		NodeData *temp = head;
		while(temp != NULL)
		{
			if(temp->Data == Data->Data)
			{
				NodeData* temp1 = temp;
				temp1->next->pre = temp1->pre;
				temp1->pre->next = temp1->next;
				delete temp1;
				break;
			}
			temp = temp->next;
		}
	}
}

void SaveData::Display()
{
	NodeData *temp = head;
	while(NULL  !=temp)
	{
		std::cout<<temp->Data<<std::endl;
		temp = temp->next;
	}
}