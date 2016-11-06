#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "macros.h"
#include "shapes.h"
#include "obj_loader.h"

#define BUFSZ 1024

static void parse_vertex(char *line, struct vec3 *v);
static void parse_face(char *line, struct vec3int *v);

void
obj_loader_init(obj_loader *obj)
{
	obj->nverts = obj->nfaces = 0;

	vector_init(&obj->vertexes, sizeof(struct vec3));
	vector_init(&obj->faces, sizeof(struct vec3int));
}

void
obj_loader_finalize(obj_loader *obj)
{
	vector_free(&obj->vertexes);
	vector_free(&obj->faces);
}

void
load_obj(obj_loader *obj, char *fname)
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
			struct vec3 v;

			parse_vertex(buf + 2, &v);
			vector_push(&obj->vertexes, &v);
		} else if (buf[0] == 'f') {
			struct vec3int f = {0};

			parse_face(buf + 2, &f);
			//printf("buf = %s face %d %d %d\n", buf, f.x, f.y, f.z);
			vector_push(&obj->faces, &f);
		}

	}
	obj->nverts = obj->vertexes.length;
	obj->nfaces = obj->faces.length;

	fclose(fp);
}

static void
parse_vertex(char *line, struct vec3 *v)
{
	sscanf(line, "%lf %lf %lf", &v->x, &v->y, &v->z);
}

static void
parse_face_row(char *line, int *p)
{
	sscanf(line, "%d", p);
}

static void
parse_face(char *line, struct vec3int *v)
{
	char *p;

	parse_face_row(line, &v->x);
	p = strchr(line, ' ');
	parse_face_row(p + 1, &v->y);
	p = strchr(p + 1, ' ');
	parse_face_row(p + 1, &v->z);
}

