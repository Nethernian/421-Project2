// Name:	Nathan Wooddell
// Email:	nathanw2@umbc.edu
// ID:		TC56612
// Description: This is a prototype for the tree required for CMSC 421
// project 2. The nodes of the tree are made up of priority queues.

// INCLUDE STATEMENTS

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>												// Specifically for error handling
#include "LinkedQueue.h"

// GLOBAL FUNCTIONS
static struct Tree_Node* PQ_Head;								// Defines the tree. 
extern int errno;												// Creates an error number field.




/* tree_Node
The tree Node is the central building block of the process tree. This node has two children
and these can either be subtrees, individual nodes, or a NULL value. The Null value indicates
that you are at the bottom of the tree. */

struct Tree_Node{
	int locked;
	unsigned long Node_ID;										// The ID/Priority of the Node
	struct Queue_Node* Node_Queue;								// A pointer to the queue
	struct Tree_Node* right;									// A pointer to the right node
	struct Tree_Node* left;										// A pointer to the left node
};










// HELPER FUNCTIONS

/* Tree_Node* Navigate(Tree_Node* N, unsigned long priority)
This is a recursive function to locate a node at a specified priority. This function should be able to work
recursively, and will return a pointer to the specified node. As a note, in the event of a node of priority
not existing, the node returned will be the closest possible node.*/

struct Tree_Node* Navigate_To(struct Tree_Node* N, unsigned long priority){
	// End Case for Recursion
	if (N->Node_ID == priority){
		return N;
		
	// Navigation Case Statement
	}else{
		// Traverse Left
		if(N->Node_ID > priority){
			// Make sure node exists
			if(N->left == NULL){
				return N;
				
			// Recursive Case
			}else{
				return Navigate_To(N->left, priority);
			}
			
		// Traverse Right
		}else if(N->Node_ID < priority){
			// Make sure node exists
			if(N->right == NULL){
				return N;
			
			// Recursive case
			}else{
				return Navigate_To(N->right, priority);
			}
		}
	}
}


/* int Recursive_Shutdown(Tree_Node* N) 
A recursive shutdown function called by the shutdown function. This will delete all of the nodes in
the tree, including calling the purge function on the queue in the node. This function will be created
to work through recursion. returns 0 on branch end.*/

int recursive_Shutdown(struct Tree_Node* N){
	if(N != NULL){
		// Traverse the tree
		recursive_Shutdown(N->right);
		recursive_Shutdown(N->left);
		
		// Clear the node, and the queue - free data
		if(N->Node_Queue != NULL){
			purge(N->Node_Queue);
		}
		free(N);
		N = NULL;
		return 0;
		
	}else{
		return 0;
	}
}









// TREE FUNCTIONS

/* long PQueue_init(void)
This function will initialize a tree, by creating a new tree node with no children and a height of
zero. The child value of the node will be set to NULL. This function should return a 0 on a sucessful
initialization. */

long PQueue_init(void){
	PQ_Head = (struct Tree_Node *)malloc(sizeof(struct Tree_Node));	// Dynamic allocation of the global head
	PQ_Head->locked = 0;
	PQ_Head->right = NULL;										// Sets the right pointer to a null
	PQ_Head->left = NULL;										// Sets the left pointer to a null
	PQ_Head->Node_ID;
}


/* long PQueue_shutdown(void)
This function will shut down the queue system, iterating through the tree, and freeing both the node,
and the content present at the pointer to priority_queue. This can be done through the use of the purge()
function in the LinkedQueue.h file.*/

long PQueue_shutdown(void){
	struct Tree_Node* temp = PQ_Head;									// Creates a temp pointer, starting at the head
	if(PQ_Head != NULL){
		recursive_Shutdown(temp);
		return 0;
	}else{
		return 1;
	}
}


/* long PQueue_create(unsigned long priority)
This will create a new queue with the specified priorty. This can only be done if the queue does not
already exist. In the event that the queue is correctly created, this function will return a 0, 
otherwise it will throw an error. */

long PQueue_create(unsigned long priority){
	// Error Handling for an unassigned PQ_Head
	if(PQ_Head == NULL){
		printf("\nNEW HEAD NODE \n");
		PQ_Head = (struct Tree_Node*)malloc(sizeof(struct Tree_Node));
		// Prepare Node Data
		PQ_Head->Node_ID = priority;
		PQ_Head->Node_Queue = NULL;
		// Prepare Children
		PQ_Head->left = NULL;
		PQ_Head->right = NULL;
		return 0;
	}
	
	struct Tree_Node* temp = PQ_Head;
	
	// Handling for determining the node placement.
	struct Tree_Node* viable = PQ_Head;							// Creates a temp pointer, starting at the head
	viable = Navigate_To(viable, priority);						// Call the Navigate to function
	
	// Check if Navigation returned a NULL
	if(viable == NULL){
		fprintf(stderr, "NULL Return from Helper: ", errno);
		return errno;
	
	// Check if the NULL already exists
	}else if(viable->Node_ID == priority){
		fprintf(stderr, "Queue Already exists: ", errno);
		return errno;
	
	// Determine placement of a new node (as child of parent node).
	}else{
		// LEFT
		if(viable->Node_ID > priority){
			printf("\nNEW LEFT NODE \n");
			viable->left = (struct Tree_Node*)malloc(sizeof(struct Tree_Node));
			viable = viable->left;
			// Set node values
			viable->Node_ID = priority;
			viable->Node_Queue = NULL;
			// Prepare Children
			viable->right = NULL;
			viable->left = NULL;
			return 0;											// Successful
			
		// RIGHT
		}else if(viable->Node_ID < priority){
			printf("\nNEW RIGHT NODE \n");
			viable->right = (struct Tree_Node*)malloc(sizeof(struct Tree_Node));
			viable = viable->right;
			// Set node values
			viable->Node_ID = priority;
			viable->Node_Queue = NULL;
			// Prepare Children
			viable->right = NULL;
			viable->left = NULL;
			return 0;											// Successful
		}
	}
		
	
}


/* long pqueue_destroy(unsigned long priority)
This will delete the pqueue identified by priority if it exists. In the event that the queue has PIDs
present in it, they should also be deleted, and then the function should return 0. Otherwise the
function should return an error. */

long PQueue_destroy(unsigned long priority){
	// Ensure the Structure has been created
	if(PQ_Head == NULL){
		fprintf(stderr, "Structure Has Not Been Initialized: ", errno);
		return errno;
	}
	// Locate the node to be deleted
	struct Tree_Node* temp = PQ_Head;
	struct Tree_Node* target = Navigate_To(temp, priority);
	
	if(target->locked >= 1){
		fprintf(stderr, "The Node is locked, and cannot be accessed")
		return errno;
	}else{
		target->locked -= 1;
	}
	
	if(target->Node_ID == priority){
		if(target->Node_Queue == NULL){
			fprintf(stderr, "Queue Does not exist - Delete considered successful");
			return 0;											// Successful
			
		}else{
			purge(target->Node_Queue);
			target->Node_Queue = NULL;
			return 0;
		}
														
	}else{
		fprintf(stderr, "The Requested Item Can't be Found: ", errno);
		return errno;
	}
}


/* long pqueue_count(unsigned long priority)
This will print the number of process IDs in the queue to the system log identified by the priority
passed to the function. This will return a 0 on a success, or an appropriate error code on a failure. */

long PQueue_count(unsigned long priority){
	// Ensure the Structure has been created
	if(PQ_Head == NULL){
		fprintf(stderr, "Structure Has Not Been Initialized: ", errno);
		return errno;
	}
		
	// Locate the node to be deleted
	struct Tree_Node* temp = PQ_Head;
	struct Tree_Node* target = Navigate_To(temp, priority);
	
	if(target->locked == 1){
			fprintf(stderr, "The Node is locked, and cannot be accessed")
			return errno;
	}else{
			target->locked = 1;
	}
		
	// Checking whether target exists
	if(target->Node_ID == priority){
		target->locked = 0;
		return count_queue(target->Node_Queue);
	}else{
		fprintf(stderr, "Specified Priority Does Not Exist: ", errno);
		target->locked = 0;
		return errno;
	}
}


/* long pqueue_send(unsigned long priority, unsigned long PID)
This function should print the numbver of PIDs in the queue to the system log identified by priority
(assuming it exists). This should then return a 0, or an appropriate error code on failure. */

long PQueue_send(unsigned long priority, unsigned long PID){
	// Ensure that the structure has been created
	if(PQ_Head == NULL){
		fprintf(stderr, "Structure Has Not Been Initialized: ", errno);
		return errno;
	}
	
	// Locate the node to be deleted
	struct Tree_Node* temp = PQ_Head;
	struct Tree_Node* target = Navigate_To(temp, priority);
	
	if(target->locked == 0){
		fprintf(stderr, "The Node is locked, and cannot be accessed")
		return errno;
	}else{
		target->locked = 1;
	}
	
	// Enqueue the PID in the Linked List at Priority
	if(target->Node_ID == priority){
		if(target->Node_Queue != NULL){
			enqueue(target->Node_Queue, PID);
			target->locked = 0;
			return 0;
		
		// Handles the queue not existing in the node
		}else{
			target->Node_Queue = Initialize(PID);
			target->locked = 0;
			return 0;
		}
		
	}else{
		fprintf(stderr, "Specified Priority Does Not Exist: ", errno);
		target->locked = 0;
		return errno;
	}
}


/* long pqueue_recv(unsigned long priority)
This function reads the process that is at the top of the FIFO queue, as identified by priority (assuming
it exists). The priority and PID are written to the system log. The PID will not be deleted. An appropriate
return code is issued if the priority does not exist. (THIS WILL BE HELPFUL FOR DEBUGGING) */

long PQueue_recv(unsigned long priority){
	// Ensure that the structure has been created
	if(PQ_Head == NULL){
		fprintf(stderr, "Structure Has Not Been Initialized: ", errno);
		return errno;
	}
		
	// Locate the node to be deleted
	struct Tree_Node* temp = PQ_Head;
	struct Tree_Node* target = Navigate_To(temp, priority);
	
	// Read from the queue if
	if(target->Node_ID == priority){
		// Successful Read
		if(target->Node_Queue != NULL){
			return recv(target->Node_Queue);
		}else{
			fprintf(stderr, "No Data at the start of the queue: ", errno);
			return errno;
		}
	}else{
		fprintf(stderr, "Specified Node Does Not Exist: ", errno);
		return errno;
	}
	
}

/* long pqueue_delete(unsigned long priority)
This function will read the PID located at the top of the FIFO queue as identified by priority (assuming 
it exists). The priority and PID are written to the system log. The PID will then be deleted. An
appropriate return code should be returned if the priority does not exist. */

long PQueue_delete(unsigned long priority){
	// Ensure that the structure has been created
	if(PQ_Head == NULL){
		fprintf(stderr, "Structure Has Not Been Initialized: ", errno);
		return errno;
	}
		
	// Locate the node to be deleted
	struct Tree_Node* temp = PQ_Head;
	struct Tree_Node* target = Navigate_To(temp, priority);
	
	// Dequeue from the queue defined by priority
	if(target->Node_ID == priority){
		if(target->Node_Queue != NULL){
			struct Queue_Node* temp = target->Node_Queue;
			target->Node_Queue = help_dq(target->Node_Queue);
			return dequeue(temp);
			
		// Handling for the Queue Not existing
		}else{
			fprintf(stderr, "Specified Priority Does Not Exist: ", errno);
			return errno;
		}
		
		
	// Handle the priority not existing
	}else{
		fprintf(stderr, "Specified Priority Does Not Exist: ", errno);
		return errno;
	}
}


// TESTING MAIN -- DO NOT BRING TO KERNEL SPACE!

int main(){
	unsigned long mid = 5;
	unsigned long high = 8;
	unsigned long low = 3;
	
	printf("\n Beginning test cases \n");

	PQueue_init();
	// Create a Head Node
	PQueue_create(mid);
	printf("Head Created Correctly \n");
	
	// Insert a lower value Node
	PQueue_create(low);
	printf("Left Value Inserted \n");
	
	// Insert a greater value Node
	PQueue_create(high);
	printf("Right Value Inserted \n");
	
	PQueue_create(mid);
	printf("Error Value Created \n");
	
	
	printf("\nTESTING BASIC QUEUE FUNCTIONS \n");
	printf("Creating Queue in head\n");
	PQueue_send(5, 762);
	PQueue_send(5, 556);
	PQueue_send(5, 308);
	PQueue_send(5, 223);

	printf("Value At Head : %lu \n", PQueue_recv(5));
	
	printf("Dequeued : %lu \n", PQueue_delete(5));
	
	printf("Next in Queue : %lu \n", PQueue_recv(5));
	printf("Dequeued : %lu \n", PQueue_delete(5));
	
	long temper = PQueue_count(5);
	printf("Total Remaining: %lu", temper);
	
	PQueue_shutdown();
	

	
	return 0;
}










