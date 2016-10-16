#include <stdio.h>

#include <cairo/cairo.h>
#include "shapes.h"

#define SURWIDTH 480
#define SURHEIGHT 360

#define ARRSZ(arr) (sizeof(arr) / sizeof(arr[0]))

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
			surdata[y * SURWIDTH + x].g = 0;
			surdata[y * SURWIDTH + x].b = 0;
			surdata[y * SURWIDTH + x].a = 0;
		}
	}
}

void
draw_cube(cairo_t *cr)
{
	struct triangle tl[] = {
		{
			{0, 0, 0},
			{100, 100, 100},
			{200, 50, 300},
		},
	};

	//WRITEME!

	/*
	
	for (i = 1; i < ARRSZ(tl); i++) {
		cairo_move_to(cr, tl[0].points[0].x, tl[0].points[0].y);
		cairo_line_to(cr, tl[i].points[i].x, tl[i].poits[i].y);
	}
	*/


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



	cairo_stroke(cr);

	cairo_surface_write_to_png(sur, "img.png");

	printf("xx\n");
	return 0;
}

