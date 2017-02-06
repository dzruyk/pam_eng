#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <cairo/cairo.h>

#include "surface.h"
#include "macros.h"

int linex0 = 0;
int liney0 = 0;

int
pe_fmtgetchans(enum SURFMT fmt)
{
	switch(fmt) {
	case SF_GRAYSCALE:
		return 1;
	case SF_RGB24:
		return 3;
	case SF_RGB32:
		return 4;
	default:
		SHOULDNT_REACH();
		return (-1);
	}
}

int
pe_surisvalid(const struct pe_surface *sur)
{
	if (sur->w <= 0 || sur->h <= 0
		|| pe_fmtgetchans(sur->fmt) <= 0 || sur->data == NULL)
		return 0;

	return 1;
}

int
pe_createsur(struct pe_surface *sur, int w, int h, enum SURFMT fmt)
{
	if (w <= 0 || h < 0 || pe_fmtgetchans(fmt) <= 0) {
		return (-1);
	}

	sur->w = w;
	sur->h = h;
	sur->fmt = fmt;

	if ((sur->data = malloc(w * h * pe_fmtgetchans(sur->fmt))) == NULL) {
		return (-1);
	}

	return 0;
}

int
pe_destroysur(struct pe_surface *sur)
{
	if (pe_surisvalid(sur)) {
		return (-1);
	}

	sur->w = 0;
	sur->h = 0;
	sur->fmt = -1;

	if (sur->data == NULL)
		free(sur->data);

	return 0;
}

int
pe_fillsur(struct pe_surface *sur, int r, int g, int b)
{
	int x, y;

	if (sur->fmt != SF_RGB24)
		return (-1);

	for (y = 0; y < sur->h; ++y)
		for (x = 0; x < sur->w; ++x) {
			sur->data[(y * sur->w + x) * 3 + 0] = r;
			sur->data[(y * sur->w + x) * 3 + 1] = g;
			sur->data[(y * sur->w + x) * 3 + 2] = b;
		}

	return 0;
}

int
pe_setpos(int x, int y)
{
	linex0 = x;
	liney0 = y;

	return 0;
}

inline int
__attribute__((always_inline))
pe_setpoint(struct pe_surface *sur, int x0, int y0, const struct pe_color *c)
{
	if (x0 >= 0 && x0 < sur->w && y0 >= 0 && y0 < sur->h) {
		sur->data[(y0 * sur->w + x0) * 3 + 0]
			= c->r * 255.0;
		sur->data[(y0 * sur->w + x0) * 3 + 1]
			= c->g * 255.0;
		sur->data[(y0 * sur->w + x0) * 3 + 2]
			= c->b * 255.0;

		return 0;
	}

	return 1;
}

int
pe_lineto(struct pe_surface *sur, int x1, int y1,
	const struct pe_color *c)
{
	int deltax, deltay;
	int error;
	int signy;
	int x0, y0;
	//int h, w;

	x0 = linex0;
	y0 = liney0;

	linex0 = x1;
	liney0 = y1;

	if (MIN(x0, x1) > sur->w ||
	    MIN(y0, y1) > sur->h ||
	    MAX(x0, x1) < 0 ||
	    MAX(y0, y1) < 0)
		return 0;

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	deltax = abs(x1 - x0);
	deltay = abs(y1 - y0);
	signy = y0 < y1 ? 1 : -1;
	error = deltax - deltay;

	//h = sur->h;
	//w = sur->w;
	while (x0 != x1 || y0 != y1) {
		int error2;

		/*
		// FIXME: оптимизация не работает корректно для линий,
		// которые начинаются за границей экрана
		if (x0 > w || y0 < 0 || y0 > h)
			break;
		*/

		pe_setpoint(sur, x0, y0, c);

		error2 = error * 2;

		if (error2 > -deltay) {
			error -= deltay;
			x0 += 1;
		}

		if (error2 < deltax) {
			error += deltax;
			y0 += signy;
		}
	}

	return 0;
}

int
pe_writesur(const struct pe_surface *sur, const char *path)
{
	cairo_surface_t *csur;
	unsigned char *surdata;
	int x, y, c;
	int chanc;

	if (!pe_surisvalid(sur) || sur->fmt != SF_RGB24) {
		return (-1);
	}

	csur = cairo_image_surface_create(CAIRO_FORMAT_RGB24, sur->w, sur->h);

	surdata = cairo_image_surface_get_data(csur);

	chanc = pe_fmtgetchans(sur->fmt);

	for (y = 0; y < sur->h; ++y)
		for (x = 0; x < sur->w; ++x)
			for (c = 0; c < chanc; ++c)
				surdata[(y * sur->w + x) * 4 + c]
					= sur->data[((sur->h - y - 1) * sur->w
						+ x) * chanc
						+ (chanc - c - 1)];

	cairo_surface_write_to_png(csur, path);

	return 0;
}
