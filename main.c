#include <stdio.h>

#include <cairo/cairo.h> 
#include "obj_loader.h"
#include "macros.h"
#include "shapes.h"
#include "matrix.h"

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

void
draw_cube(cairo_t *cr)
{
	struct triangle tl[] = {
		{
			{
				{1, 1, 1},
				{100, 1, 1},
				{1, 100, 1}
			}
		},
		{
			{
				{100, 1, 1},
				{100, 100, 1},
				{1, 100, 1}
			}
		}
	};

	int i, j;

	//WRITEME!
	cairo_set_source_rgb(cr, 1, 0, 0);

	for (i = 0; i < ARRSZ(tl); i++) {
		struct vec3 *p = tl[i].points;
		printf("vec3\n");

		cairo_move_to(cr, p[0].x, p[0].y);
		for (j = 1; j <= 3; j++) {
			int tmp = j % 3;
			cairo_line_to(cr, p[tmp].x, p[tmp].y);
		}
		cairo_stroke(cr);
	}
	cairo_stroke(cr);
}

void
draw_model(cairo_t *cr, struct mat3 h)
{
	obj_loader obj;
	int i, j;

	obj_loader_init(&obj);

	load_obj(&obj, "african_head.obj");
	cairo_set_source_rgb(cr, 0, 0, 0);
	// Flip y coordinate for upside-down source image
	for (i = 0; i < obj.nfaces; i++) {
		struct vec3int *f;
		struct vec3 a;
		int x, y;
		int *arr;

		f = stack_get(&obj.faces, i);
		a = *(struct vec3 *)stack_get(&obj.vertexes, f->x - 1);
		a = mat3vec(h, a);
		arr = &f->x;

		x = ((a.x + 1.) * SURWIDTH) / 2;
		y = ((-a.y + 1.) * SURWIDTH) / 2;
		cairo_move_to(cr, x, y);
		for (j = 0; j < 3; j++) {
			int idx;

			idx = arr[(j + 1) % 3];
			a = *(struct vec3 *)stack_get(&obj.vertexes, idx - 1);
			a = mat3vec(h, a);
			x = ((a.x + 1.) * SURWIDTH) / 2;
			y = ((-a.y + 1.) * SURWIDTH) / 2;
			cairo_line_to(cr, x, y);
		}
		cairo_stroke(cr);

	}
	cairo_stroke(cr);

	obj_loader_finalize(&obj);
}

int
main(int argc, const char *argv[])
{
	cairo_t *cr;
	cairo_surface_t *sur;
	struct rgba *surdata;

	sur = cairo_image_surface_create(CAIRO_FORMAT_RGB24, SURWIDTH, SURHEIGHT);

	surdata = (struct rgba *)cairo_image_surface_get_data(sur);

	fill_surf(surdata);

	cr = cairo_create(sur);

	//draw_cube(cr);

	draw_model(cr, mat3mult(mat3scale(1.2, 1.2), mat3rotate(-0.53, 1))); // give matrix that multiplies model

	cairo_surface_write_to_png(sur, "img.png");

	printf("xx\n");
	return 0;
}

