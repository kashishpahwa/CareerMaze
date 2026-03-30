#ifndef STACK_H
#define STACK_H
#include<stdio.h>
#include<stdlib.h>

struct stack {
    void *ptr;
    struct stack *next;
};
typedef struct stack stack;

void push(stack **top,void *p){
    stack *t=(stack*)malloc(sizeof(stack));
    t->ptr=p;
    t->next=*top;
    *top=t;
}

void* pop(stack **top){
    if(*top==NULL) return NULL;

    stack *t=*top;
    void *p=t->ptr;
    *top=t->next;
    free(t);
    return p;
}

#endif