#ifndef __MESH_H__
#define __MESH_H__

#include "dbuf.h"
#include "matrix.h"

enum edge_type {
	TRIANGLE = 0,
};

struct vertex {
	struct vec4 pos;
	struct vec4 norm;
};

struct mesh {
	enum edge_type type;

	struct dbuf vertex; //dbuf of struct vertex items
	struct dbuf idx;    //dbuf of int items
};

void pe_meshinit(struct mesh *m);

void pe_meshnormalize(struct mesh *m);

void pe_meshclean(struct mesh *m);

#endif
