/*
 * Sam Whalen and Zack Smith
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <ucontext.h>
#include "list.h"
#include "threadsalive.h"
#define STACKSIZE 128000

/* ***************************** 
     stage 1 library functions
   ***************************** */

static thread *head; 
static thread *current;
static ucontext_t main;
static int blocked;


void ta_libinit(void) {
    head = NULL;
    current = NULL;
    blocked = 0;
}

void ta_create(void (*func)(void *), void *arg) {
	//make the stack
	unsigned char *stack = (unsigned char *)malloc(STACKSIZE);
	ucontext_t *context = malloc(sizeof(ucontext_t));
	assert(stack);

	getcontext(context);
	(context->uc_stack).ss_sp = stack;
	(context->uc_stack).ss_size = STACKSIZE;
	context->uc_link = &main;
	//add the thread to the list
	list_add(context, &head);
	makecontext(context, (void (*)(void))func, 1, arg);
}

void ta_yield(void) {
	thread *tmp = current;
	if (current->next != NULL){
		//pick the next thread		
		current = current->next;
	}
	else{
		//go to the front of the list
		current = head;
	}
	swapcontext(tmp->context, current->context);	
}

int ta_waitall(void) {
    while (head->next != NULL){
	thread *next;
	if (current == NULL){
		next = head;
	}
	else if (current == head){
		next = current->next;
		//take off of the list
		list_delete(current, &head);
	}
	else{
		next = head;
		//take off of the list
		list_delete(current, &head);
	}
	current = next;
	swapcontext(&main, current->context);
    }
    //nothing is on the list anymore
    list_delete(head, &head);
    return 0;
}


/* ***************************** 
     stage 2 library functions
   ***************************** */

void ta_sem_init(tasem_t *sem, int val) {
	sem->val = val;
	sem->wait = NULL;
}

void ta_sem_destroy(tasem_t *sem) {

}

void ta_sem_post(tasem_t *sem) { //wake up a thread if there's any waiting
	sem->val+=1;
	thread *awake = pop(&sem->wait);//get the first one in the list
	if (awake != NULL){//if there is nothing in the list		
		list_push(awake, &head);
		blocked-=1;
	}	
}

void ta_sem_wait(tasem_t *sem) { 
	if (sem->val <= 0){ //wait for sem
		list_move(current, &head);
		list_push(current, &sem->wait);
		blocked+=1;
		ta_yield();
	}
	sem->val-=1;
}

void ta_lock_init(talock_t *mutex) {
	mutex->sem = malloc(sizeof(tasem_t));
	ta_sem_init(mutex->sem, 1);
}

void ta_lock_destroy(talock_t *mutex) {

}

void ta_lock(talock_t *mutex) {
	ta_sem_wait(mutex->sem);
}

void ta_unlock(talock_t *mutex) {
	ta_sem_post(mutex->sem);
}


/* ***************************** 
     stage 3 library functions
   ***************************** */

void ta_cond_init(tacond_t *cond) {
	cond->sem = malloc(sizeof(tasem_t));
	ta_sem_init(cond->sem, 0);
}

void ta_cond_destroy(tacond_t *cond) {

}

void ta_wait(talock_t *mutex, tacond_t *cond) {
	ta_unlock(mutex);
	ta_sem_wait(cond->sem);
	ta_lock (mutex);
}

void ta_signal(tacond_t *cond) {
	ta_sem_post(cond->sem);
}





