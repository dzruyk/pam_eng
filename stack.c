#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "stack.h"


int
stack_init(stack *Vector, int size) {

	Vector->size = size;
	Vector->length = 0;
	Vector->capacity = STACK_CAPACITY;
	Vector->flag = 0;

	unsigned char *tmp = malloc(Vector->size * Vector->capacity);

	if (!tmp)               /* malloc returned no memory signal */
		return 1;

	Vector->data = tmp;

	return 0;
}

void
stack_initarr(stack *Vector, unsigned char *arr, int length, int size) {

        assert(arr != NULL);

        Vector->length = length;
        Vector->capacity = length - 1;
        Vector->flag = 1;
        Vector->size = size;

        Vector->data = arr;
}

int
stack_len(stack *Vector) { 

	assert(Vector != NULL);

	return Vector->length;
}

void*
stack_getarr(stack *Vector) {

	assert(Vector != NULL);

	return Vector->data;
}

void*
stack_get(stack *Vector, int index) {

	assert(Vector != NULL);
	assert(-1 < index);
	assert(index < Vector->length);

	return Vector->data + index * Vector->size;
}

void
stack_set(stack *Vector, int index, void *value) {

	assert(Vector != NULL);
	assert(0 <= index);
	assert(index < Vector->length);

	memcpy(Vector->data + index * Vector->size, value, Vector->size);
}

int
stack_push(stack *Vector, void *value) {

	assert(Vector != NULL);

	if (Vector->length == Vector->capacity) {

		if (Vector->flag)
			return 1;		/* can't reallocate given array */

		unsigned char *tmp;

		Vector->capacity *= STACK_CAPACITY_GROWTH;
		tmp = realloc(Vector->data, Vector->size * Vector->capacity);

		if (!tmp)
			return 1;		/* realloc returned no memory signal */

		Vector->data = tmp;

	}

	stack_set(Vector, Vector->length++, value);

	return 0;
}

void*
stack_pop(stack *Vector) {

	assert(Vector != NULL);
	assert(Vector->length != 0);

	return Vector->data + Vector->length-- * Vector->size;
}

void
stack_free(stack *Vector) {

	assert(Vector != NULL);
	assert(Vector->data != NULL);

	free(Vector->data);
}
