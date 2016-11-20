#ifndef _SHAPES_H__
#define _SHAPES_H__

#include "dbuf.h"
#include "matrix.h"

enum edge_type {
	TRIANGLE = 0,
};

struct mesh {
	enum edge_type type;

	dbuf vertex;
	dbuf idx;
};

void mesh_init(struct mesh *m);

void mesh_normalize(struct mesh *m);

void mesh_clean(struct mesh *m);

#endif
