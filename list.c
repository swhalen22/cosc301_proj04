#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <string.h>
#include <ucontext.h>

/* template from proj01  */

void list_add(ucontext_t *context, thread **head) {
	thread *new = malloc(sizeof(thread));
	thread *tmp = *head;
	new->context = context;
	new->next = NULL;
	if (*head == NULL){
		*head = new;
	}
	else{
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = new;
	}
}

void list_push(thread *thd, thread **head){
	thread *tmp = *head;
	if (*head == NULL){
		*head = thd;
	}
	else{
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = thd;
	}
}

int list_delete(thread *context, thread **head) {
	thread *tmp = *head;
	if (tmp == context){
		*head = tmp->next;
		free(((tmp->context)->uc_stack).ss_sp);
		free(tmp->context);
		free(tmp);
		return 1;
	}
	tmp = tmp->next;
	thread *prev = *head;
	while (tmp != NULL){
		if (context == tmp){
			prev->next = tmp->next;
			free(((tmp->context)->uc_stack).ss_sp);
			free(tmp->context);
			free(tmp);
			return 1;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return 0;
}

void list_clear(thread *list) { //adjusted from hw03
    while (list != NULL) {
        thread *tmp = list;
        list = list->next;
		free(((tmp->context)->uc_stack).ss_sp);
		free(tmp->context);
        free(tmp);
    }
}


void list_move(thread *thd, thread **head){
	thread *tmp = *head;
	if (tmp == thd){
		*head = tmp->next;
		tmp->next = NULL;
		return;
	}
	tmp = tmp->next;
	thread *prev = *head;
	while (tmp != NULL){
		if (tmp == thd){
			prev->next = tmp->next;
			tmp->next = NULL;
			return;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}


thread *pop(thread **head){//take the first thing from the list
	if (*head == NULL){
		return NULL;
	}
	thread *tmp = *head;
	*head = tmp->next;
	tmp->next = NULL;
	return tmp;
}
