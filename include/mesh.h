#ifndef __MESH_H__
#define __MESH_H__

#include "dbuf.h"
#include "matrix.h"

enum edge_type {
	TRIANGLE = 0,
};

struct mesh {
	enum edge_type type;

	struct dbuf vertex;
	struct dbuf idx;
};

void pe_meshinit(struct mesh *m);

void pe_meshnormalize(struct mesh *m);

void pe_meshclean(struct mesh *m);

#endif
