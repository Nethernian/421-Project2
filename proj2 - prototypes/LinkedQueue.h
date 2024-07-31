// Name:	Nathan Wooddell
// Email:	Nathanw2@umbc.edu
// ID:		TC56612
// Description:	This is the header file for the project2 queue. Both this and the LinkedQueue.c file are
// now needed.

#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

// Struct Definition block
struct QueueNode;

// Function Declaration block
struct Queue_Node* Initialize(unsigned long data);

void enqueue(struct Queue_Node* q, unsigned long data);

unsigned long dequeue(struct Queue_Node* q);

void purge(struct Queue_Node* q);

unsigned long recv(struct Queue_Node* q);

int count_queue(struct Queue_Node* q);

struct Queue_Node* help_dq(struct Queue_Node* q);


#endif