#include "ManagerList.h"

ManagerList::ManagerList()
	: head(0)
{
}

ManagerList::~ManagerList()
{
}

void ManagerList::add(Node* inNode, unsigned int id)
{
	//add to the front of the linked list
	inNode->id = id;
	inNode->next = head;
	inNode->prev = 0;

	head = inNode;
	if(inNode->next != 0)
	{
		inNode->next->prev = inNode;
	}
}

void ManagerList::remove(unsigned int id)
{
	//find node
	Node* tmp = head;
	while(tmp != 0 && tmp->id != id)
	{
		tmp = tmp->next;
	}

	//remove node if found;
	if(tmp != 0)
	{
		if(tmp ->prev != 0)
		{
			tmp->prev->next = tmp->next;
		}
		if(tmp->next != 0)
		{
			tmp->next->prev = tmp->prev;
		}
		delete tmp;
	}
}

void ManagerList::clear()
{

	while (head != 0)
	{
		Node* tmp = head;
		head = head->next;
		delete tmp;
	}
}

Node* ManagerList::get(unsigned int id)
{
	Node* tmp = head;
	while(tmp != 0 && tmp->id != id)
	{
		tmp = tmp->next;
	}

	return tmp;
}