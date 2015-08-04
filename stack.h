/*
 * Stack header file
 */

#ifndef STACK_H_
#define STACK_H_

#define STACK_SIZE 50 

typedef struct{
	int top;
	int items[STACK_SIZE];
} stackType;

//Function prototypes.
void initStack(stackType * s);
void stackPush(stackType *, int x);
int stackPop(stackType *);
void printStack(stackType *);

#endif
