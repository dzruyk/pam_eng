#ifndef _STACK_
#define _STACK_

#define STACK_CAPACITY 8
#define STACK_CAPACITY_GROWTH 2

/*
    VECTOR_CAPACITY is conctant of a begining stack size
    VECTOR_CAPACITY_GROWTH is a conctant of stack's growth rate
*/

typedef
struct
{
	unsigned int length;        /* the number of used stack units */
	unsigned int capacity;      /* the size of stack in stack units */
	unsigned int size;          /* stack unit size */
	unsigned char *data;        /* stack addres */
	int flag;                   /* flag == TRUE, if data wasn't allocated */
} stack;

int
stack_init(stack *Vector, int size);

void
stack_initarr(stack *Vector, unsigned char *arr, int length, int size);

int
stack_len(stack *Vector);

void*
stack_getarr(stack *Vector);

void*
stack_get(stack *Vector, int index);

void
stack_set(stack *Vector, int index, void *value);

int
stack_push(stack *Vector, void *value);

void*
stack_pop(stack *Vector);

void
stack_free(stack *Vector);

#endif
