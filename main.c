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
	obj_loader obj;

	if (argc < 3)
		usage(argv[0]);

	obj_loader_init(&obj);

	obj_loader_load(&obj, argv[1]);

	if (pe_createsur(&sur, 720, 480, SF_RGB24) < 0) {
		return (-1);
	}

	pe_initcontext(&context);
	pe_settarget(&context, &sur);
	pe_setvertex(&context, (const dbuf *) &(obj.vertexes));
	pe_setindex(&context, (const dbuf *) &(obj.faces));

	obj_loader_normalize(&obj);

	pe_render(&context);

	obj_loader_finalize(&obj);

	pe_writesur(&sur, argv[2]);

	return 0;
}

