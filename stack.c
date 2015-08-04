/**
 * stack.c
 * This file will contain the implementation of the stack.
 * In the case of wren the only thing the stack holds is
 * integer values, so this can be treated as an array with
 * a pointer (index) which moves back and forth along it.
 *
 * @author Dr. Fenwick & Dakota Murray
 * @version Spring 2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

void initStack(stackType * s)
{
    s->top = -1;
}

void stackPush(stackType *s, int item)
{
    if(s->top == (STACK_SIZE - 1))
    {
        printf("Stack push error: stack is full.\nInterpreter terminating.\n");
	    exit(1);
    }
    else {
        s->top += 1;
        s->items[s->top] = item;
    }
}

int stackPop(stackType *s)
{
    int toReturn;
    if (s->top == -1) {
        printf("Stack pop error: stack is empty.\nInterpreter terminating.\n");
        exit(1);
    } else {
        toReturn = s->items[s->top];
        s->top -= 1;
    }
    return toReturn;
}

void printStack(stackType * s)
{
   int i;
   if (s->top == -1)
   {
      printf("Stack is empty\n");
      return;
   }
   for (i  = s->top; i >= 0; i--)
      printf("%d\n", s->items[i]);
}
