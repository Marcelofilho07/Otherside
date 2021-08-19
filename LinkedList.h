/*
Code by: Marcelo Aragão e Silva Filho
GitHub: https://github.com/Marcelofilho07
*/

#include "Node.h"


class LinkedList
{
public:
	int length;
	Node* head;

	LinkedList();
	~LinkedList();
	void appendToHead(Entity* data);
	void appendToTail(Entity* data);
};