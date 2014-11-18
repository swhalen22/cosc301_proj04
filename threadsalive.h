/*
 * Sam Whalen and Zack Smith
 */

#ifndef __THREADSALIVE_H__
#define __THREADSALIVE_H__
#include "list.h"

/* ***************************
        type definitions
   *************************** */

typedef struct {
	int val;
	thread *wait;
} tasem_t;

typedef struct {
	tasem_t *sem;
} talock_t;

typedef struct {
	tasem_t *sem;
} tacond_t;


/* ***************************
       stage 1 functions
   *************************** */

void ta_libinit(void);
void ta_create(void (*)(void *), void *);
void ta_yield(void);
int ta_waitall(void);

/* ***************************
       stage 2 functions
   *************************** */

void ta_sem_init(tasem_t *, int);
void ta_sem_destroy(tasem_t *);
void ta_sem_post(tasem_t *);
void ta_sem_wait(tasem_t *);
void ta_lock_init(talock_t *);
void ta_lock_destroy(talock_t *);
void ta_lock(talock_t *);
void ta_unlock(talock_t *);

/* ***************************
       stage 3 functions
   *************************** */

void ta_cond_init(tacond_t *);
void ta_cond_destroy(tacond_t *);
void ta_wait(talock_t *, tacond_t *);
void ta_signal(tacond_t *);

#endif /* __THREADSALIVE_H__ */
