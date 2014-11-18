#ifndef __LIST_H__
#define __LIST_H__
#include <ucontext.h>

typedef struct node {
    ucontext_t *context;
    struct node *next; 
}thread;

void list_add(ucontext_t *, thread **);
void list_push(thread *, thread **);
int list_delete(thread *, thread **);
void list_clear(thread *);
void list_move(thread *, thread **);
thread *pop(thread **);

#endif // __LIST_H__
