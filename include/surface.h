#ifndef __SURFACE_H__
#define __SURFACE_H__

struct pe_color {
	double r;
	double g;
	double b;
	double a;
};

enum SURFMT {
	SF_GRAYSCALE,
	SF_RGB24,
	SF_RGB32
};

struct pe_surface
{
	int w;
	int h;
	enum SURFMT fmt;
	unsigned char *data;
};

int pe_createsur(struct pe_surface *sur, int w, int h, enum SURFMT fmt);

int pe_destroysur(struct pe_surface *sur);

int pe_fillsur(struct pe_surface *sur, int r, int g, int b);

int pe_setpos(int x, int y);

int pe_getpoint(const struct pe_surface *sur, double fx, double fy,
	struct pe_color *col);

int pe_setpoint(struct pe_surface *sur, int x0, int y0, const struct pe_color *c);

int pe_lineto(struct pe_surface *sur, int x1, int y1,
	const struct pe_color *c);

int pe_surfromfile(const char *surpath, struct pe_surface *sur);

int pe_writesur(const struct pe_surface *sur, const char *path);

#endif
