#include <stdio.h>

#include <cairo/cairo.h> 

#include "obj_loader.h"
#include "macros.h"
#include "shapes.h"
#include "matrix.h"
#include "render.h"

#define SURWIDTH  4096
#define SURHEIGHT 4096

struct rgba {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
};

void
fill_surf(struct rgba *surdata)
{
	int x, y;

	for (y = 0; y < SURHEIGHT; ++y) {
		for (x = 0; x < SURWIDTH; ++x) {
			surdata[y * SURWIDTH + x].r = 255;
			surdata[y * SURWIDTH + x].g = 255;
			surdata[y * SURWIDTH + x].b = 255;
			surdata[y * SURWIDTH + x].a = 0;
		}
	}
}

int
draw_model(const char *path)
{
	struct pe_context context;
	struct pe_surface sur;
	obj_loader obj;
	
	obj_loader_init(&obj);

	load_obj(&obj, path);

	if (pe_createsur(&sur, 720, 480, SF_RGB24) < 0) {
		return (-1);
	}

	pe_initcontext(&context);
	pe_settarget(&context, &sur);
	pe_setvertex(&context, (const dbuf *) &(obj.vertexes));
	pe_setindex(&context, (const dbuf *) &(obj.faces));	

	pe_render(&context);

	obj_loader_finalize(&obj);

	pe_writesur(&sur, "1.png");

	return 0;
}

int
main(int argc, const char *argv[])
{
	struct pe_context context;
	struct pe_surface sur;
	obj_loader obj;

	if (argc < 3)
		printf("usage: runit [input .obj] [output .png]\n");
	
	obj_loader_init(&obj);

	load_obj(&obj, argv[1]);

	if (pe_createsur(&sur, 720, 480, SF_RGB24) < 0) {
		return (-1);
	}

	pe_initcontext(&context);
	pe_settarget(&context, &sur);
	pe_setvertex(&context, (const dbuf *) &(obj.vertexes));
	pe_setindex(&context, (const dbuf *) &(obj.faces));	

	pe_render(&context);

	obj_loader_finalize(&obj);

	pe_writesur(&sur, argv[2]);

	return 0;
}

