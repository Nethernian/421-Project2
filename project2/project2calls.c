// Name:	Nathan Wooddell
// Email:	nathanw2@umbc.edu
// ID:		TC56612
// Description: This is the system call file for my project 2. All items contained in this file are
// system calls, and are intended to be implemented in kernel space. This file will be a culmination
// of both the PQueue_Tree.c, and the LinkedQueue.c files.

// INCLUDE STATEMENTS
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>



// GLOBAL VARIABLES 
static struct BST_N* PQ_Head;





// NODE DEFINITIONS

/* BST_N
This is the main node type for the binary search tree. It includes a lock, a pointer to the head
of the PID queue, it's priority, and pointers to it's children. */

struct BST_N{
	unsigned long N_priority;														// The priority of the node in the tree
	int locked;																		// Used in locking the node from editing
	struct Queue_N* N_Queue;														// A pointer to the PID queue
	struct BST_N* right;															// A pointer to the right BST node
	struct BST_N* left;																// A pointer to the left BST node
};


/* Queue_N
This is a node for the linked list that makes up the PID queue, which is stored in each node. This
node contains a pointer to the next node in the array, and the process ID.*/

struct Queue_N{
	unsigned long PID;																// The ID of the process represented by the node
	struct Queue_N* next;															// A pointer to the next node in the queue
};





// HELPER FUNCTIONS

/* struct N_Queue* Init_Queue(unsigned long data)
This will intialize a Priority Queue with the value passed trough the data variable. This is the
expected way a program will create a new queue when a node in the tree is created. This should only
run on node creation/first send to a node. */

struct Queue_N* Init_Queue(unsigned long data){
	struct Queue_N* initial;
	
	// CODE
	
	initial = (struct Queue_N*)kmalloc(sizeof(struct Queue_N), GFP_KERNEL);
	initial->next = NULL;
	initial->PID = data;
	return initial;
}


/* void Queue_Del(struct Queue_N* Q)
This function deletes all data from a process ID queue. It performs this operation recursively,
and returns no values. This function is used by the Help_Delete function to clear the queues of
the nodes. */

void Queue_Del(struct Queue_N* q){
	struct Queue_N* temp;
	
	// CODE
	
	// Loops until the final node
	while(q->next != NULL){
		// Creates a temporary node to be deleted
		temp = q;
		q = q->next;
		// Free the temporary node
		kfree(temp);
	}
	// Frees the final node
	kfree(q);
}


/* struct BST_N* Get_Node(struct BST_N* Node, unsigned long priority)
This function is a recursive search function, it will navigate throug the tree, and find the node
matching the priority passed to the function(or the closest node to that point). This will then be
returned upwards through recursion. */

struct BST_N* Get_Node(struct BST_N* Node, unsigned long priority){
	// End Case for Recursion
	if (Node->N_priority == priority){
		return Node;
		
	// Navigation Case Statement
	}else{
		// Traverse Left
		if(Node->N_priority > priority){
			// Make sure node exists
			if(Node->left == NULL){
				return Node;
				
			// Recursive Case
			}else{
				return Get_Node(Node->left, priority);
			}
			
		// Traverse Right
		}else if(Node->N_priority < priority){
			// Make sure node exists
			if(Node->right == NULL){
				return Node;
			
			// Recursive case
			}else{
				return Get_Node(Node->right, priority);
			}
		}
		return NULL;
	}	
}


/* int Help_Delete(struct BST_N* Node)
This function is meant to assist in deleting the binary search tree, and all of the associated
PID queues recursively. This function relies on the free helper function, which is responsible
for freeing all nodes in the PID queue. */

int Help_Delete(struct BST_N* Node){
	if(Node != NULL){
		// Traverse the tree
		Help_Delete(Node->right);
		Help_Delete(Node->left);
		
		// Clear the node, and the queue - free data
		if(Node->N_Queue != NULL){
			Queue_Del(Node->N_Queue);
		}
		kfree(Node);
		Node = NULL;
		return 0;
		
	}else{
		return 0;
	}
}


/* struct N_Queue* Get_Next(struct N_Queue* q)
This function was created to squash a strange edge case, which was arising in the PQueue_Delete()
function. This function simply returns the next value in the queue. */

struct Queue_N* Get_Next(struct Queue_N* q){
	return q->next;
}


/* int count_queue(struct Queue_N* q)
This is a helper function for the pqueue_count syscall. The way in which this is written is a
bit of a holdover from my userspace document, and could be implemented without an additional
funtion. */

int count_queue(struct Queue_N* q){
	int count = 0;
	
	// CODE
	
	while(q != NULL){
		q = q->next;
		count++;
	}
	return count;
}





// QUEUE FUNCTIONS


/* void PID_Enqueue(struct N_Queue* q, unsigned long PID)
This is the enqueue function for the FIFO queue of process IDs, It will create a new object at the
end of the queue, which will remain there until it is dequeued. */

void PID_Enqueue(struct Queue_N* q, unsigned long PID){
	struct Queue_N* new_node;
	
	// CODE
	
	// Loop through the linked list until you reach the end
	while(q->next != NULL){
		// Iterate through linked list
		q = q->next;	
	}
	
	// Handle the nonexisting node pointer.
	if(q->next == NULL){
		// Create new variable for the node
		new_node = NULL;
		// Dynamically allocate a new node
		new_node = (struct Queue_N*)kmalloc(sizeof(struct Queue_N), GFP_KERNEL);
		// Insert the data
		new_node->PID = PID;
		new_node->next = NULL;
		// Place the new node as the next node
		q->next = new_node;
		return;
		
	// Special Error Handling -> Hopefully Redundant
	}else{
		return;
	}
}


/* unsigned long PID_dequeue(struct N_Queue* q)
This is the dequeue function for the the FIFO queue. It will be used in the delete function to
return the top of the queue to the user. The return value of this function will be the data
contained in the head of the queue. The queue will be updated by this function to represent the
deletion. */

unsigned long PID_dequeue(struct Queue_N* q){
	unsigned long t_value;
	struct Queue_N* temp;
	
	// CODE
	
	// create a temp to hold the former head.
	temp = q;
	// Move the head down one spot.
	q = q->next;
	// save data before deletion.
	t_value = temp->PID;
	// Clear data and return data
	kfree(temp);
	return t_value;
}



// BINARY SEARCH TREE FUNCTIONS





// SYSTEM CALLS


/* long pqueue_init(void)
This function is used to initialize the pqueue structure. This will return a 0 on a success, and
will return an error from the errno.h header on a failure. This will create the root node of the
tree, but will not assign it any data. */

SYSCALL_DEFINE0(pqueue_init){
	if(PQ_Head == NULL){
		PQ_Head = (struct BST_N*)kmalloc(sizeof(struct BST_N), GFP_KERNEL);	// Dynamic allocation of the global head
		PQ_Head->right = NULL;										// Sets the right pointer to a null
		PQ_Head->left = NULL;										// Sets the left pointer to a null
		PQ_Head->locked = 0;
		return 0;
	}else{
		return -EEXIST; 
	}
}

SYSCALL_DEFINE0(pqueue_shutdown){
	struct BST_N* temp;									// Creates a temp pointer, starting at the head
	
	// CODE
	if(PQ_Head != NULL){
		temp = PQ_Head;
		Help_Delete(temp);
		return 0;
	}else{
		return -ENODEV;
	}
	
}

SYSCALL_DEFINE1(pqueue_create, unsigned long, priority){
	struct BST_N* viable;

	// CODE
	
	// Handling for determining the node placement.
	viable = PQ_Head;											// Creates a temp pointer, starting at the head
	viable = Get_Node(viable, priority);						// Call the Navigate to function
	
	if(viable->locked == 1){
		return -EPERM;
	}else{
		viable->locked = 1;
	}
	
	// Check if Navigation returned a NULL
	if(viable == NULL){
		printk("NULL Return from Helper\n");
		viable->locked = 0;
		return -ENOMEM;
	
	// Check if the NULL already exists
	}else if(viable->N_priority == priority){
		printk("Queue Already exists\n");
		viable->locked = 0;
		return -EEXIST;
	
	// Determine placement of a new node (as child of parent node).
	}else{
		// LEFT
		if(viable->N_priority > priority){
			viable->left = (struct BST_N*)kmalloc(sizeof(struct BST_N), GFP_KERNEL);
			viable->locked = 0;
			viable = viable->left;
			// Set node values
			viable->N_priority = priority;
			viable->N_Queue = NULL;
			// Prepare Children
			viable->right = NULL;
			viable->left = NULL;
			return 0;											// Successful
			
		// RIGHT
		}else if(viable->N_priority < priority){
			viable->right = (struct BST_N*)kmalloc(sizeof(struct BST_N), GFP_KERNEL);
			viable->locked = 0;
			viable = viable->right;
			// Set node values
			viable->N_priority = priority;
			viable->N_Queue = NULL;
			// Prepare Children
			viable->right = NULL;
			viable->left = NULL;
			return 0;											// Successful
		}else{
			return -EFAULT;
		}
	}
}

SYSCALL_DEFINE1(pqueue_destroy, unsigned long, priority){
	struct BST_N* temp;
	struct BST_N* target;
	
	// CODE
	
	// Ensure the Structure has been created
	if(PQ_Head == NULL){
		printk("Structure Has Not Been Initialized\n");
		return -ENOENT;
	}
	// Locate the node to be deleted
	temp = PQ_Head;
	target = Get_Node(temp, priority);
	
	if(target->locked == 1){
		return -EPERM;
	}else{
		target->locked = 1;
	}
	
	if(target->N_priority == priority){
		if(target->N_Queue == NULL){
			printk("Queue Does not exist\n");
			target->locked = 0;
			return -ENOENT;											// Successful
			
		}else{
			Queue_Del(target->N_Queue);
			target->N_Queue = NULL;
			target->locked = 0;
			return 0;
		}
														
	}else{
		printk("The Requested Item Can't be Found\n");
		target->locked = 0;
		return -ENOENT;
	}	
}

SYSCALL_DEFINE1(pqueue_count, unsigned long, priority){
	struct BST_N* temp = PQ_Head;
	struct BST_N* target;
	
	// CODE
	
	// Ensure the Structure has been created
	if(PQ_Head == NULL){
		printk("Structure Has Not Been Initialized\n");
		return -ENODEV;
	}
		
	// Locate the node to be counted
	temp = PQ_Head;
	target = Get_Node(temp, priority);
	
	// Locking Block
	if(target->locked == 1){
		return -EPERM;
	}else{
		target->locked = 1;
	}
		
	// Checking whether target exists
	if(target->N_priority == priority){
		target->locked = 0;
		return count_queue(target->N_Queue);
	}else{
		printk("Specified Priority Does Not Exist \n");
		target->locked = 0;
		return -ENOENT;
	}
}

SYSCALL_DEFINE2(pqueue_send, unsigned long, priority, unsigned long, PID){
	struct BST_N* temp = PQ_Head;
	struct BST_N* target;
	// CODE
	
	// Ensure that the structure has been created
	if(PQ_Head == NULL){
		printk("Structure Has Not Been Initialized\n");
		return -ENODEV;
	}
	
	// Locate the node to be deleted
	temp = PQ_Head;
	target = Get_Node(temp, priority);
	
	// Locking Block
	if(target->locked == 1){
		return -EPERM;
	}else{
		target->locked = 1;
	}
	
	// Enqueue the PID in the Linked List at Priority
	if(target->N_priority == priority){
		if(target->N_Queue != NULL){
			PID_Enqueue(target->N_Queue, PID);
			target->locked = 0;
			return 0;
		
		// Handles the queue not existing in the node
		}else{
			target->N_Queue = Init_Queue(PID);
			target->locked = 0;
			return 0;
		}
		
	}else{
		printk("Specified Priority Does Not Exist\n");
		target->locked = 0;
		return -ENOENT;
	}	
}


SYSCALL_DEFINE1(pqueue_recv, unsigned long, priority){
	struct BST_N* temp;
	struct BST_N* target;
	// CODE
	
	// Ensure that the structure has been created
	if(PQ_Head == NULL){
		printk("Structure Has Not Been Initialized");
		return -ENODEV;
	}
			
	// Locate the node to be deleted
	temp = PQ_Head;
	target = Get_Node(temp, priority);
	
	// Locking Block
	if(target->locked == 1){
		return -EPERM;
	}else{
		target->locked = 1;
	}
		
	// Read from the queue if
	if(target->N_priority == priority){
		// Successful Read
		if(target->N_Queue != NULL){
			target->locked = 0;
			return Get_Next(target->N_Queue)->PID;
		}else{
			printk("No Data at the start of the queue: ");
			target->locked = 0;
			return -ENOENT;
		}
	}else{
		printk("Specified Node Does Not Exist");
		target->locked = 0;
		return -ENOENT;
	}
}


SYSCALL_DEFINE1(pqueue_delete, unsigned long, priority){
	struct BST_N* temp;
	struct Queue_N* temp2;
	struct BST_N* target;
	// CODE
	
	// Ensure that the structure has been created
	if(PQ_Head == NULL){
		printk("Structure Has Not Been Initialized: ");
		return -ENODEV;
	}
		
	// Locate the node to be deleted
	temp = PQ_Head;
	target = Get_Node(temp, priority);
	
	// Locking Block
	if(target->locked == 1){
		return -EPERM;
	}else{
		target->locked = 1;
	}
	
	// Dequeue from the queue defined by priority
	if(target->N_priority == priority){
		if(target->N_Queue != NULL){
			temp2 = target->N_Queue;
			target->N_Queue = Get_Next(target->N_Queue);
			target->locked = 0;
			return PID_dequeue(temp2);
			
		// Handling for the Queue Not existing
		}else{
			printk("Specified Queue Does Not Exist\n");
			target->locked = 0;
			return -ENOENT;
		}
		
		
	// Handle the priority not existing
	}else{
		printk("Specified Priority Does Not Exist\n");
		target->locked = 0;
		return -ENOENT;
	}
}

