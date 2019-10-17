

#ifndef Queue_H_
#define  Queue_H_

#include<iostream>
#include<cstdlib>

#define MAX_SIZE 10

class Queue{
private:
	int item[MAX_SIZE];
	int rear;
	int front;
public:
	Queue();
	void enqueue(int);
	int dequeue();
	int size();
	void display();
	bool isEmpty();
	bool isFull();
};
#endif



