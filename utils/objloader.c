#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "mesh.h"
#include "objloader.h"

#define BUFSZ 1024

static void parse_vertex(const char *line, struct vec4 *v);
static int parse_face(char *line, struct vec3int *v);

void
pe_objload(struct mesh *m, const char *fname)
{
	FILE *fp;
	char buf[BUFSZ];
	int i;

	assert(m != NULL);

	fp = fopen(fname, "rb");
	if (fp == NULL)
		error(1, "can't fopen file '%s'", fname);

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (buf[1] != ' ')
			continue;

		if (buf[0] == 'v') {
			struct vec4 v = {.arr = {0}};

			parse_vertex(buf + 2, &v);
			dbuf_push(&m->vertex, &v);
		} else if (buf[0] == 'f') {
			struct vec3int f = {.arr = {0}};

			parse_face(buf + 2, &f);
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

static void
parse_face_row(const char *line, int *p)
{
	sscanf(line, "%d", p);
}

static int
parse_face(char *line, struct vec3int *v)
{
	int i;
	char *p;
	char *d = " \t";

	for (i = 0; i < 3; i++) {
		p = strtok(line, d);
		if (p == NULL) {
			warning("can't find triangle index %d", i);
			return 1;
		}
		parse_face_row(p, &v->arr[i]);

		//set line to NULL, for next strtok(3) calls
		line = NULL;
	}

	return 0;
}

