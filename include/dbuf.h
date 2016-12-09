#ifndef __DBUF__
#define __DBUF__

#define DBUF_CAPACITY 8
#define DBUF_CAPACITY_GROWTH 2

/*
    VECTOR_CAPACITY is a constant of the begining dbuf size
    VECTOR_CAPACITY_GROWTH is a conctant of dbuf's growth rate
*/

struct dbuf
{
	unsigned int length;        /* the number of used dbuf units */
	unsigned int capacity;      /* the size of dbuf in dbuf units */
	unsigned int size;          /* dbuf unit size */
	unsigned char *data;        /* dbuf addres */
	int flag;                   /* flag == TRUE, if data wasn't allocated */
};

int dbuf_init(struct dbuf *Dbuf, int size);

void dbuf_initarr(struct dbuf *Dbuf, unsigned char *arr, int length, int size);

int dbuf_len(const struct dbuf *Dbuf);

void *dbuf_getarr(const struct dbuf *Dbuf);

int dbuf_ensure(struct dbuf *db, unsigned int size);

void *dbuf_get(const struct dbuf *Dbuf, int index);

void dbuf_set(struct dbuf *Dbuf, int index, void *value);

int dbuf_push(struct dbuf *Dbuf, void *value);

void *dbuf_pop(struct dbuf *Dbuf);

void dbuf_free(struct dbuf *Dbuf);

#endif
