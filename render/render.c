#include "render.h"

void
pe_safefree(void **p)
{
	if (*p != NULL) {
		free(*p);
		*p = NULL;
	}
}

int
pe_initcontext(struct pe_context *c)
{
	c->conf.usematerial = 0;
	c->conf.usetexture = 0;

	c->worldmat = mat4identity();
	c->perspmat = mat4identity();

	c->mat = NULL;
	c->texture = NULL;

	c->vertex = NULL;
	c->index = NULL;

	return 0;
}

int
pe_settarget(struct pe_context *c, struct pe_surface *sur)
{
	if (c == NULL || sur == NULL)
		return (-1);

	c->target = sur;

	return 0;
}

int
pe_setconfig(struct pe_context *c, const struct pe_contextconf *conf)
{
	if (c == NULL || conf == NULL)
		return (-1);

	c->conf = *conf;

	return 0;
}

int
pe_destroycontext(struct pe_context *c)
{
	return 0;
}

int
pe_setworldmatrix(struct pe_context *c, struct mat4 *m)
{
	if (c == NULL || m == NULL)
		return (-1);

	c->worldmat = *m;

	return 0;
}

int
pe_setperspmatrix(struct pe_context *c, struct mat4 *m)
{
	if (c == NULL || m == NULL)
		return (-1);

	c->worldmat = *m;

	return 0;
}

int
pe_setvertex(struct pe_context *c, const dbuf *v)
{
	if (c == NULL || v == NULL)
		return (-1);

	c->vertex = v;

	return 0;
}

int
pe_setindex(struct pe_context *c, const dbuf *idx)
{
	if (c == NULL || idx == NULL)
		return (-1);

	c->index = idx;

	return 0;
}

int
pe_setmaterial(struct pe_context *c, const struct pe_material *mat)
{
	if (c == NULL || mat == NULL)
		return (-1);

	c->mat = mat;

	return 0;
}

int
pe_settexture(struct pe_context *c, const struct pe_surface *sur)
{
	if (c == NULL || sur == NULL)
		return (-1);

	c->texture = sur;

	return 0;
}

static int
wiredrender(const struct pe_context *c)
{
	int i, j;

	for (i = 0; i < c->index->length; i += 3) {
		int *pidx;
		struct vec4 a;
		int x, y;

		pidx = dbuf_get(c->index, i);
		a = *(struct vec4 *)dbuf_get(c->vertex, pidx[0] - 1);
		a.w = 1.0;
		a = mat4vec(c->worldmat, a);

		x = ((a.x + 1.) * c->target->w) / 2;
		y = ((a.y + 1.) * c->target->h) / 2;

		pe_setpos(x, y);

		for (j = 0; j < 3; j++) {
			int idx;

			// Traverse each vertex of triangle
			idx = pidx[(j + 1) % 3];

			a = *(struct vec4 *)dbuf_get(c->vertex, idx - 1);
			a.w = 1.0;
			a = mat4vec(c->worldmat, a);
			x = ((a.x + 1.) * c->target->w) / 2;
			y = ((a.y + 1.) * c->target->h) / 2;

			pe_lineto(c->target, x, y);
		}

	}

	return 0;
}

int
pe_render(struct pe_context *c)
{
	wiredrender(c);

	return 0;
}

