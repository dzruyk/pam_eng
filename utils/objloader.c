#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "mesh.h"
#include "objloader.h"

#define BUFSZ 1024


static void parse_vertex(const char *line, struct vec4 *v);
static int parse_face(const char *line, struct pe_vidx *v);

void
pe_objload(struct mesh *m, const char *fname)
{
	struct vec4 v;
	struct vec4 va;
	struct pe_vidx f[3];
	FILE *fp;
	char buf[BUFSZ];
	int i, nnorm, ntexture, npoints;

	assert(m != NULL);

	npoints = nnorm = ntexture = 0;

	fp = fopen(fname, "rb");
	if (fp == NULL)
		error(1, "can't fopen file '%s'", fname);


	while (fgets(buf, sizeof(buf), fp) != NULL) {

		if (buf[0] == 'v') {

			if (buf[1] == ' ') {
				npoints++;
				memset(&v, 0, sizeof(v));

				parse_vertex(buf + 2, &v);
				dbuf_push(&m->vertex, &v);

			} else if (buf[1] == 'n' && buf[2] == ' ') {
				memset(&va, 0, sizeof(va));

				parse_vertex(buf + 3, &va);
				//push as vec3
				dbuf_push(&m->norm, &va);
				nnorm++;
			} else if (buf[1] == 't' && buf[2] == ' ') {
				memset(&va, 0, sizeof(va));

				parse_vertex(buf + 3, &va);
				//push as vec3
				dbuf_push(&m->text, &va);
				ntexture++;
			}

		} else if (buf[0] == 'f' && buf[1] == ' ') {
			memset(&f, 0, sizeof(f));

			parse_face(buf + 1, f);
			for (i = 0; i < 3; i++)
				dbuf_push(&m->idx, &f[i]);
		}
	}

	printf("OBJFILE:\n"
	       "texture points = %d\n"
	       "norm points = %d\n"
	       "points = %d\n", ntexture, nnorm, npoints);

	fclose(fp);
}

static void
parse_vertex(const char *line, struct vec4 *v)
{
	sscanf(line, "%lf %lf %lf", &v->x, &v->y, &v->z);
}

//TODO: parse normals and textures vectors
static int
parse_face_row(const char *l, struct pe_vidx *v)
{
	char *p;
	char *d = "/ \t";

	p = (char *)l;

	v->v = v->n = v->t = -1;

	//OBJ initail idx is 1, decrement it
	// v
	v->v = atoi(l) - 1;
	p = l + strcspn(l, d);
	if (p[0] != '/')
		goto end;


	// v/t
	if (p[1] != '/') {
		v->t = atoi(p + 1) - 1;
		p += strcspn(l, d);
		if (p[0] != '/')
			goto end;
	} else {
		// v//n
		p++;
	}

	// v/t/n
	v->n = atoi(p + 1);

end:
	p += strcspn(p, " \t");

	return p - l;
}

static int
parse_face(const char *line, struct pe_vidx *v)
{
	int i, n;
	const char *p;
	char *d = " \t";

	p = line;
	//printf("%s\n", line);
	for (i = 0; i < 3; i++) {
		n = strspn(p, d);
		if (n == 0 && i != 2) {
			warning("can't find triangle index %d, s = '%s'", i, p);
			exit(42);
			return 1;
		}
		p += n;
		p += parse_face_row(p, &v[i]);
	}

	return 0;
}

