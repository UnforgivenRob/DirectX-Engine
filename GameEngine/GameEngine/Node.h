#ifndef NODE_H
#define NODE_H

class ManagerList;

class Node
{
	friend ManagerList;
public:
	Node()
		: prev(0), next(0)
	{
	}

	virtual ~Node()
	{
	}
	Node* getNext()
	{
		return next;
	}
	Node* getPrev()
	{
		return prev;
	}
protected:
	Node* prev;
	Node* next;
	unsigned int id;
private:
};
#endif