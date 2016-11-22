#include <assert.h>
#include <stdlib.h>

#include "dbuf.h"
#include "mesh.h"

void
pe_meshinit(struct mesh *m)
{
	dbuf_init(&m->vertex, sizeof(struct vec4));
	dbuf_init(&m->idx, sizeof(int));
}

void
pe_meshnormalize(struct mesh *m)
{
	int i, nverts;
	struct vec4 *v;
	double lx, ly;
	double ux, uy;
	double cx, cy;
	double dx, dy;
	double dmax;

	v = (struct vec4 *) dbuf_get(&m->vertex, 0);
	lx = ux = v->x;
	ly = uy = v->y;

	nverts = m->vertex.length;
	for (i = 1; i < nverts; i++) {
		v = (struct vec4 *) dbuf_get(&m->vertex, i);

		assert(v != NULL);

		if (lx > v->x)
			lx = v->x;
		if (ly > v->y)
			ly = v->y;
		if (ux < v->x)
			ux = v->x;
		if (uy < v->y)
			uy = v->y;
	}

	cx = (lx + ux) / 2;
	cy = (ly + uy) / 2;

	dx = ux - lx;
	dy = uy - ly;
	dmax = dx;

	if (dy > dmax)
		dmax = dy;
	dmax /= 2;

	assert(dmax != 0);

	for (i = 0; i < nverts; i++) {
		v = (struct vec4 *) dbuf_get(&m->vertex, i);
		v->x = (v->x - cx) / dmax;
		v->y = (v->y - cy) / dmax;
	}
}

void
pe_meshclean(struct mesh *m)
{
	dbuf_free(&m->vertex);
	dbuf_free(&m->idx);
}

