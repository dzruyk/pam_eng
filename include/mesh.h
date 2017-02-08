#ifndef __MESH_H__
#define __MESH_H__

#include "dbuf.h"
#include "matrix.h"

enum edge_type {
	TRIANGLE = 0,
};

struct pe_vidx {
	union {
		struct {
			int v;
			int n;
			int t;
		};
		int arr[3];
	};
};

struct mesh {
	enum edge_type type;

	struct dbuf vertex;	//vertex cooridates, struct vec4
	struct dbuf norm;	//normale coordinates
	struct dbuf text;	//texture coordinates
	struct dbuf idx;	//idx information, struct pe_vidx
};

void pe_meshinit(struct mesh *m);

void pe_meshnormalize(struct mesh *m);

void pe_meshclean(struct mesh *m);

#endif
