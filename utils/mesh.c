#include <assert.h>
#include <stdlib.h>

#include "dbuf.h"
#include "mesh.h"

void
pe_meshinit(struct mesh *m)
{
	dbuf_init(&m->vertex, sizeof(struct vertex));
	dbuf_init(&m->idx, sizeof(int));
}

void
pe_meshnormalize(struct mesh *m)
{
	int i, nverts;
	struct vertex *v;
	double lx, ly;
	double ux, uy;
	double cx, cy;
	double dx, dy;
	double dmax;

	v = (struct vertex *) dbuf_get(&m->vertex, 0);
	lx = ux = v->pos.x;
	ly = uy = v->pos.y;

	nverts = m->vertex.length;
	for (i = 1; i < nverts; i++) {
		v = (struct vertex *) dbuf_get(&m->vertex, i);

		assert(v != NULL);

		if (lx > v->pos.x)
			lx = v->pos.x;
		if (ly > v->pos.y)
			ly = v->pos.y;
		if (ux < v->pos.x)
			ux = v->pos.x;
		if (uy < v->pos.y)
			uy = v->pos.y;
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
		v = (struct vertex *) dbuf_get(&m->vertex, i);
		v->pos.x = (v->pos.x - cx) / dmax;
		v->pos.y = (v->pos.y - cy) / dmax;
	}
}

void
pe_meshclean(struct mesh *m)
{
	dbuf_free(&m->vertex);
	dbuf_free(&m->idx);
}

