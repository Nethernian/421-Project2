// Name:	Nathan Wooddell
// Email:	nathanw2@umbc.edu
// ID:		TC56612
// Description: This will be the code for the queues, which will be held in the priority tree nodes.
// This queue is currently written under the assumption that it should be FIFO, not LIFO.

#include <stdlib.h>
#include <stdio.h>

/* Queue definition */

/* queue_Node
This is the definition for a queue_Node, the queue Node stores the process ID value, and a
pointer to the next node in the process queue. By itself this does nothing, and relies on 
an assignment from the initialization statement. */

struct Queue_Node {
	unsigned long data;											// Replace Later with PID
	struct Queue_Node* next;									// Points to the next node in the Linked List
};


/* Queue_Node initialize()
This function will create a new empty linked list queue. This is going to be called when a new PID
is added to the queue. This will also be an essential part of the setup of a queue in the starting
tree node. */

struct Queue_Node* Initialize(unsigned long data){
	struct Queue_Node* initial;
	initial = (struct Queue_Node*)malloc(sizeof(struct Queue_Node));
	initial->next = NULL;
	initial->data = data;
	return initial;
}


/* void enqueue(queue_Node)
This function is used to add a Process ID to the queue, this should be called from the pointer
to the head of the linked list. This function will use alloc/malloc to create a new node, and 
later insert a new PID. */

void enqueue(struct Queue_Node* q, unsigned long data){
	// Loop through the linked list until you reach the end
	while(q->next != NULL){
		// Iterate through linked list
		q = q->next;	
	}
	
	// Handle the nonexisting node pointer.
	if(q->next == NULL){
		// Create new variable for the node
		struct Queue_Node* new_node = NULL;
		// Dynamically allocate a new node
		new_node = (struct Queue_Node*)malloc(sizeof(struct Queue_Node));
		// Insert the data
		new_node->data = data;
		new_node->next = NULL;
		// Place the new node as the next node
		q->next = new_node;
		return;
		
	// Special Error Handling -> Hopefully Redundant
	}else{
		return;
	}
}


/* unisgned long dequeue(queue_Node)
This function is meant to delete the node from the head of the linked list. The lists are
usually stored by their head, and the linked list will ideally return the value stored within
the linked list node. */

unsigned long dequeue(struct Queue_Node* q){
	// create a temp to hold the former head.
	struct Queue_Node* temp = q;
	// Move the head down one spot.
	q = q->next;
	// save data before deletion.
	unsigned long t_value = temp->data;
	// Clear data and return data
	free(temp);
	return t_value;
}


/* void purge(queue_Node)
This function will purge the entire linked list, top to bottom without saving or displaying the
data contained within the data. All dynamically allocated data should be freed during this step. */

void purge(struct Queue_Node* q){
	// Loops until the final node
	while(q->next != NULL){
		// Creates a temporary node to be deleted
		struct Queue_Node* temp = q;
		q = q->next;
		// Free the temporary node
		free(temp);
	}
	// Frees the final node
	free(q);
}


/* unsigned long recv(Queue_Node* q)
display the top value of the queue object passed by the node q.
 */
unsigned long recv(struct Queue_Node* q){
	return q->data;
}


/* int count_queue(struct Queue_Node* q)
count the number of items in the queue pointed to by q. This is expected to be the head of the
linked list. Returns the count as an Integer. */
int count_queue(struct Queue_Node* q){
	int count = 0;
	while(q != NULL){
		q = q->next;
		count++;
	}
	return count;
}


// HELPER FUNCTIONS

/* struct Queue_Node* help_dq(Queue_Node* q)
gather the next node after the current head. This will be used in the dequeue operation*/

struct Queue_Node* help_dq(struct Queue_Node* q){
	return q->next;
}
