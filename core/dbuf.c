#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dbuf.h"


int
dbuf_init(dbuf *Dbuf, int size) {

	Dbuf->size = size;
	Dbuf->length = 0;
	Dbuf->capacity = DBUF_CAPACITY;
	Dbuf->flag = 0;

	unsigned char *tmp = malloc(Dbuf->size * Dbuf->capacity);

	if (tmp == NULL)               /* malloc returned no memory signal */
		return 1;

	Dbuf->data = tmp;

	return 0;
}

void
dbuf_initarr(dbuf *Dbuf, unsigned char *arr, int length, int size) {

        assert(arr != NULL);

        Dbuf->length = length;
        Dbuf->capacity = length - 1;
        Dbuf->flag = 1;
        Dbuf->size = size;

        Dbuf->data = arr;
}

int
dbuf_len(const dbuf *Dbuf) {

	assert(Dbuf != NULL);

	return Dbuf->length;
}

void*
dbuf_getarr(const dbuf *Dbuf) {

	assert(Dbuf != NULL);

	return Dbuf->data;
}

void*
dbuf_get(const dbuf *Dbuf, int index) {

	assert(Dbuf != NULL);
	assert(-1 < index);
	assert(index < Dbuf->length);

	return Dbuf->data + index * Dbuf->size;
}

void
dbuf_set(dbuf *Dbuf, int index, void *value) {

	assert(Dbuf != NULL);
	assert(0 <= index);
	assert(index < Dbuf->length);

	memcpy(Dbuf->data + index * Dbuf->size, value, Dbuf->size);
}

int
dbuf_push(dbuf *Dbuf, void *value) {

	assert(Dbuf != NULL);

	if (Dbuf->length == Dbuf->capacity) {

		if (Dbuf->flag)
			return 1;		/* can't reallocate given array */

		unsigned char *tmp;

		Dbuf->capacity *= DBUF_CAPACITY_GROWTH;
		tmp = realloc(Dbuf->data, Dbuf->size * Dbuf->capacity);

		if (!tmp)
			return 1;		/* realloc returned no memory signal */

		Dbuf->data = tmp;

	}

	dbuf_set(Dbuf, Dbuf->length++, value);

	return 0;
}

void*
dbuf_pop(dbuf *Dbuf) {

	assert(Dbuf != NULL);
	assert(Dbuf->length != 0);

	return Dbuf->data + Dbuf->length-- * Dbuf->size;
}

void
dbuf_free(dbuf *Dbuf) {

	assert(Dbuf != NULL);
	assert(Dbuf->data != NULL);

	free(Dbuf->data);
}
