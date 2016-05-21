#ifndef MANAGER_H
#define MANAGER_H

#include "Node.h"

class ManagerList
{
public:
	ManagerList();
	~ManagerList();

	void add(Node* inNode, unsigned int id);
	void remove(unsigned int id);
	void clear();
	Node* get(unsigned int id);
	Node* head;
	
};
#endif