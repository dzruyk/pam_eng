#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdlib.h>

#include "dbuf.h"
#include "matrix.h"
#include "surface.h"

struct pe_material {
	struct pe_color color;
};

struct pe_material pe_defmat;

struct pe_contextconf {
	int wired;
};

struct pe_context
{
	struct pe_contextconf conf;
	struct pe_surface *target;
	struct pe_surface *zbuffer;

	struct mat4 worldmat;
	struct mat4 perspmat;
	struct mat4 viewportmat;

	const struct pe_material *mat;
	const struct pe_surface *texture;

	const struct dbuf *vertex;
	const struct dbuf *index;
	const struct dbuf *norm;
	const struct dbuf *texcoord;
};

int pe_initcontext(struct pe_context *c);

int pe_settarget(struct pe_context *c, struct pe_surface *sur);

int pe_setconfig(struct pe_context *c, const struct pe_contextconf *conf);

int pe_destroycontext(struct pe_context *c);

int pe_setworldmatrix(struct pe_context *c, struct mat4 *m);

int pe_setperspmatrix(struct pe_context *c, struct mat4 *m);

int pe_setvertex(struct pe_context *c, const struct dbuf *v);

int pe_setindex(struct pe_context *c, const struct dbuf *idx);

int pe_setnorm(struct pe_context *c, const struct dbuf *norm);

int pe_settexcoord(struct pe_context *c, const struct dbuf *texcoord);

int pe_setmaterial(struct pe_context *c, const struct pe_material *mat);

int pe_settexture(struct pe_context *c, const struct pe_surface *sur);

int pe_render(struct pe_context *c);

#endif
