#include <stdio.h>
#include <string.h>

#include <libgen.h>

#include "obj_loader.h"
#include "macros.h"
#include "shapes.h"
#include "matrix.h"
#include "render.h"

char *usg = "usage: %s [OPTS] input.obj output.png\n";

void
usage(const char *progpath)
{
	char *p, *pname;

	p = strdup(progpath);

	if (p == NULL)
		pname = (char *) progpath;
	else
		pname = basename(p);

	printf(usg, pname);

	exit(1);
}

int
main(int argc, const char *argv[])
{
	struct pe_context context;
	struct pe_surface sur;
	struct mesh m;
	struct pe_material mat;

	if (argc < 3)
		usage(argv[0]);

	mesh_init(&m);

	obj_loader_load(&m, argv[1]);

	if (pe_createsur(&sur, 720, 480, SF_RGB24) < 0) {
		return (-1);
	}

	mat.color.r = 0.0;
	mat.color.g = 1.0;
	mat.color.b = 0.0;
	mat.color.a = 1.0;

	pe_initcontext(&context);
	pe_settarget(&context, &sur);
	pe_setvertex(&context, (const dbuf *) &(m.vertex));
	pe_setindex(&context, (const dbuf *) &(m.idx));
	pe_setmaterial(&context, &mat);

	mesh_normalize(&m);

	pe_render(&context);

	mesh_clean(&m);

	pe_writesur(&sur, argv[2]);

	return 0;
}

