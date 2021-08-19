/*
Code by: Marcelo Aragão e Silva Filho
GitHub: https://github.com/Marcelofilho07
*/

#include "LinkedList.h"
#include<iostream>

LinkedList::LinkedList()
{
	this->length = 0;
	this->head = NULL;
}

LinkedList::~LinkedList()
{
	std::cout << "DELETED" << std::endl;
}

void LinkedList::appendToHead(Entity* data)
{
	Node* node = new Node();
	node->data = data;
	node->next = this->head;
	this->head = node;
	this->length++;
}

void LinkedList::appendToTail(Entity* data)
{
	Node* tail = new Node();
	tail->data = data;
	tail->next = NULL;
	Node* n = this->head;

	while (n->next != NULL)
		n = n->next;

	n->next = tail;
	this->length++;
}