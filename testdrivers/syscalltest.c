// Name:	Nathan Wooddell
// Email:	nathanw2@umbc.edu
// ID:		TC56612
// Description: This is a testing driver for the system calls that I created as a part of project2.
// This driver will test the basic functions of the binary search tree, and the queue attached to
// it.

// INCLUDE STATEMENT BLOCK

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>


// SYSCALL DEFINITIONS

#define __NR_pqueue_init 548
#define __NR_pqueue_shutdown 549
#define __NR_pqueue_create 550
#define __NR_pqueue_destroy 551
#define __NR_pqueue_count 552
#define __NR_pqueue_send 553
#define __NR_pqueue_recv 554
#define __NR_pqueue_delete 555


// A VERY general main, my syscalls will be called from here.
int main(int argc, char *argv[]){
	
}