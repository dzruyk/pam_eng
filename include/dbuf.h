#ifndef _DBUF_
#define _DBUF_

#define DBUF_CAPACITY 8
#define DBUF_CAPACITY_GROWTH 2

/*
    VECTOR_CAPACITY is conctant of a begining dbuf size
    VECTOR_CAPACITY_GROWTH is a conctant of dbuf's growth rate
*/

typedef
struct
{
	unsigned int length;        /* the number of used dbuf units */
	unsigned int capacity;      /* the size of dbuf in dbuf units */
	unsigned int size;          /* dbuf unit size */
	unsigned char *data;        /* dbuf addres */
	int flag;                   /* flag == TRUE, if data wasn't allocated */
} dbuf;

int dbuf_init(dbuf *Dbuf, int size);

void dbuf_initarr(dbuf *Dbuf, unsigned char *arr, int length, int size);

int dbuf_len(const dbuf *Dbuf);

void *dbuf_getarr(const dbuf *Dbuf);

void *dbuf_get(const dbuf *Dbuf, int index);

void dbuf_set(dbuf *Dbuf, int index, void *value);

int dbuf_push(dbuf *Dbuf, void *value);

void *dbuf_pop(dbuf *Dbuf);

void dbuf_free(dbuf *Dbuf);

#endif
