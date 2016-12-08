#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dbuf.h"

int
dbuf_init(struct dbuf *Dbuf, int size)
{
	unsigned char *tmp;

	Dbuf->size = size;
	Dbuf->length = 0;
	Dbuf->capacity = DBUF_CAPACITY;
	Dbuf->flag = 0;

	tmp = malloc(Dbuf->size * Dbuf->capacity);

	if (tmp == NULL) /* malloc returned no memory signal */
		return 1;

	Dbuf->data = tmp;

	return 0;
}

void
dbuf_initarr(struct dbuf *db, unsigned char *arr, int length, int size) {

        assert(arr != NULL);

        db->length = length;
        db->capacity = length - 1;
        db->flag = 1;
        db->size = size;

        db->data = arr;
}

int
dbuf_len(const struct dbuf *db)
{
	assert(db != NULL);

	return db->length;
}

void *
dbuf_getarr(const struct dbuf *db)
{
	assert(db != NULL);

	return db->data;
}

void *
dbuf_get(const struct dbuf *db, int index)
{
	assert(db != NULL);
	assert(-1 < index);
	assert(index < db->length);

	return db->data + index * db->size;
}

void
dbuf_set(struct dbuf *db, int index, void *value)
{
	assert(db != NULL);
	assert(0 <= index);
	assert(index < db->length);

	memcpy(db->data + index * db->size, value, db->size);
}

int
dbuf_push(struct dbuf *db, void *value)
{
	assert(db != NULL);

	if (db->length == db->capacity) {
		unsigned char *tmp;
		unsigned int ncap;

		if (db->flag)
			return 1; /* can't reallocate given array */

		ncap = db->capacity * DBUF_CAPACITY_GROWTH;

		tmp = realloc(db->data, db->size * ncap);
		if (tmp == NULL)
			return 1; /* nomem */

		db->capacity = ncap;
		db->data = tmp;
	}

	dbuf_set(db, db->length++, value);

	return 0;
}

void *
dbuf_pop(struct dbuf *db)
{
	assert(db != NULL);
	assert(db->length != 0);

	return db->data + db->length-- * db->size;
}

void
dbuf_free(struct dbuf *db)
{
	assert(db != NULL);
	assert(db->data != NULL);

	free(db->data);
}
