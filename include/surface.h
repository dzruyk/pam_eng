#ifndef SURFACE_H
#define SURFACE_H

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

int pe_lineto(struct pe_surface *sur, int x, int y);

int pe_writesur(const struct pe_surface *sur, const char *path);

#endif
