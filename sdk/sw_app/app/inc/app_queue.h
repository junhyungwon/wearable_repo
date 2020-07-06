#ifndef APP_QUEUE_H
#define APP_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct tagNode
{
	char *Data;
        int Size ;
        int ch ;
        int type ; // normal, event
	struct tagNode *NextNode;
}Node;

typedef struct tagLinkedQueue
{
	Node *Front;
	Node *Rear;
	int Count;
}LinkedQueue;

void LQ_CreateQueue(LinkedQueue **Queue);
void LQ_DestroyQueue(LinkedQueue **Queue);

Node *LQ_CreateNode(char *Data, int ch, int Size, int type);
void LQ_DestroyNode(Node *_Node);
void LQ_Enqueue(LinkedQueue *Queue, Node* NewNode);
Node *LQ_Dequeue(LinkedQueue *Queue);
int LQ_IsEmpty(LinkedQueue *Queue);

#endif
