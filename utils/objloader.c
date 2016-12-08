#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "mesh.h"
#include "objloader.h"

#define BUFSZ 1024

struct vert_idx {
	struct vec3int v;
	struct vec3int n;
	struct vec3int t;
};

static void parse_vertex(const char *line, struct vec4 *v);
static int parse_face(const char *line, struct vec3int *v);

void
pe_objload(struct mesh *m, const char *fname)
{
	struct vertex v;
	struct vec4 va;
	struct vec3int f;
	struct dbuf bnorm;
	FILE *fp;
	char buf[BUFSZ];
	int i, nnorm, npoints;

	assert(m != NULL);

	dbuf_init(&bnorm, sizeof(struct vec4));

	npoints = nnorm = 0;

	fp = fopen(fname, "rb");
	if (fp == NULL)
		error(1, "can't fopen file '%s'", fname);


	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (buf[1] != ' ')
			continue;

		if (buf[0] == 'v') {

			if (buf[1] == ' ') {
				npoints++;
				memset(&v, 0, sizeof(v));

				parse_vertex(buf + 2, &v.pos);
				dbuf_push(&m->vertex, &v);

			} else if (buf[1] == 'n') {

				parse_vertex(buf + 3, &va);
				dbuf_push(&bnorm, &va);
				nnorm++;
			}

		} else if (buf[0] == 'f') {
			memset(&f, 0, sizeof(f));

			parse_face(buf + 1, &f);
			for (i = 0; i < 3; i++)
				dbuf_push(&m->idx, &f.arr[i]);
		}
	}

	fclose(fp);
}

static void
parse_vertex(const char *line, struct vec4 *v)
{
	sscanf(line, "%lf %lf %lf", &v->x, &v->y, &v->z);
}

//TODO: parse normals and textures vectors
static int
parse_face_row(const char *l, int *idx)
{
	const char *p;
	int ret;
	int n, t;

	p = l;

	*idx = atoi(l);
	*idx -= 1;	//OBJ initail idx == 1, decrement it

	p += strcspn(l, " \t");

	return p - l;
}

static int
parse_face(const char *line, struct vec3int *v)
{
	int i, n;
	const char *p;
	char *d = " \t";

	p = line;
	printf("%s\n", line);
	for (i = 0; i < 3; i++) {
		n = strspn(p, d);
		if (n == 0 && i != 2) {
			warning("can't find triangle index %d, s = '%s'", i, p);
			exit(42);
			return 1;
		}
		p += n;
		p += parse_face_row(p, &v->arr[i]);
	}

	return 0;
}

