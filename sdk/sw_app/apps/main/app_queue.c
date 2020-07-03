#include <string.h>
#include "app_queue.h"



void LQ_CreateQueue(LinkedQueue **Queue)
{
    (*Queue) = (LinkedQueue *)malloc(sizeof(LinkedQueue));
    (*Queue)->Front = NULL;
    (*Queue)->Rear = NULL;
    (*Queue)->Count = 0;
}

void LQ_DestroyQueue(LinkedQueue **Queue)
{
    while((*Queue)->Count)
    {
        Node *Popped = LQ_Dequeue(*Queue);
        LQ_DestroyNode(Popped);
    }

    free(*Queue) ;
}

Node *LQ_CreateNode(char *Data, int ch, int Size, int type)
{
    Node *NewNode = (Node *)malloc(sizeof(Node));
    NewNode->Data = (char *)malloc(Size) ;
    memcpy(NewNode->Data, Data, Size) ;
    NewNode->Size = Size ;
    NewNode->ch = ch ;
    NewNode->type = type ;
    NewNode->NextNode = NULL;

    return NewNode ; 
}


void LQ_DestroyNode(Node *_Node)
{
    free(_Node->Data) ;
    free(_Node);
}

void LQ_Enqueue(LinkedQueue *Queue, Node *NewNode)
{
    if(Queue->Front == NULL)
    {
	Queue->Front = NewNode;
	Queue->Rear = NewNode;
	Queue->Count++;
    }
    else
    {
	Queue->Rear->NextNode = NewNode;
	Queue->Rear = NewNode;
	Queue->Count++;
    }
}

Node *LQ_Dequeue(LinkedQueue *Queue)
{
    Node* Front = Queue->Front;

    if(Queue->Front->NextNode == NULL)
    {
	Queue->Front = NULL;
	Queue->Rear = NULL;
    }
    else
    {
	Queue->Front = Queue->Front->NextNode;
    }

    Queue->Count--;

    return Front;
}

int LQ_IsEmpty(LinkedQueue *Queue)
{
//    return (Queue->Front == NULL);
      if(Queue->Front != NULL)
          return 0 ;
      else
          return 1 ;
}
