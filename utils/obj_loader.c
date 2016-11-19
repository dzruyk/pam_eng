#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbuf.h"
#include "macros.h"
#include "shapes.h"
#include "obj_loader.h"

#define BUFSZ 1024

static void parse_vertex(const char *line, struct vec4 *v);
static void parse_face(const char *line, struct vec3int *v);

void
obj_loader_init(obj_loader *obj)
{
	obj->nverts = obj->nfaces = 0;

	dbuf_init(&obj->vertexes, sizeof(struct vec4));
	dbuf_init(&obj->faces, sizeof(struct vec3int));
}

void
obj_loader_finalize(obj_loader *obj)
{
	dbuf_free(&obj->vertexes);
	dbuf_free(&obj->faces);
}

void
obj_loader_load(obj_loader *obj, const char *fname)
{
	FILE *fp;
	char buf[BUFSZ];

	fp = fopen(fname, "rb");
	if (fp == NULL)
		error(1, "can't fopen file '%s'", fname);

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (buf[1] != ' ')
			continue;

		if (buf[0] == 'v') {
			struct vec4 v = {0};

			parse_vertex(buf + 2, &v);
			dbuf_push(&obj->vertexes, &v);
		} else if (buf[0] == 'f') {
			struct vec3int f = {0};

			parse_face(buf + 2, &f);
			dbuf_push(&obj->faces, &f);
		}

	}
	obj->nverts = obj->vertexes.length;
	obj->nfaces = obj->faces.length;

	fclose(fp);
}

void
obj_loader_normalize(obj_loader *obj)
{
	int i;
	struct vec4 *v;
	double lx, ly;
	double ux, uy;
	double cx, cy;
	double dx, dy;
	double dmax;

	assert(obj->nverts > 0);

	v = (struct vec4 *) dbuf_get(&obj->vertexes, 0);
	lx = ux = v->x;
	ly = uy = v->y;

	for (i = 1; i < obj->nverts; i++) {
		v = (struct vec4 *) dbuf_get(&obj->vertexes, i);

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

	for (i = 0; i < obj->nverts; i++) {
		v = (struct vec4 *) dbuf_get(&obj->vertexes, i);
		v->x = (v->x - cx) / dmax;
		v->y = (v->y - cy) / dmax;
	}
}

static void
parse_vertex(const char *line, struct vec4 *v)
{
	sscanf(line, "%lf %lf %lf", &v->x, &v->y, &v->z);
}

static void
parse_face_row(const char *line, int *p)
{
	sscanf(line, "%d", p);
}

static void
parse_face(const char *line, struct vec3int *v)
{
	char *p;

	parse_face_row(line, &v->x);
	p = strchr(line, ' ');
	parse_face_row(p + 1, &v->y);
	p = strchr(p + 1, ' ');
	parse_face_row(p + 1, &v->z);
}

