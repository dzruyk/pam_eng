#include "render.h"

struct pe_material pe_defmat = { {1.0, 1.0, 1.0, 1.0} };

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
	c->conf.usetexture = 0;

	mat4identity(&c->worldmat);
	mat4identity(&c->perspmat);

	c->mat = &pe_defmat;
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

	c->perspmat = *m;

	return 0;
}

int
pe_setvertex(struct pe_context *c, const struct dbuf *v)
{
	if (c == NULL || v == NULL)
		return (-1);

	c->vertex = v;

	return 0;
}

int
pe_setindex(struct pe_context *c, const struct dbuf *idx)
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
	int objsz;
	struct mat4 res;

	pe_setperspmatrix((struct pe_context *)c, mat4persp(&res, 1, 100, -1, 1, -1, 1));

	objsz = c->target->w;

	if (c->target->h < objsz)
		objsz = c->target->h;

	for (i = 0; i < c->index->length; i += 3) {
		int *pidx;
		struct vec4 *pa, tmp;
		int x, y;

		pidx = dbuf_get(c->index, i);
		pa = dbuf_get(c->vertex, pidx[0] - 1);
		pa->w = 1.0;

		mat4vec(&tmp, &c->worldmat, pa);
		pa = mat4vec(&tmp, &c->perspmat, &tmp);

		pa->x /= pa->w;
		pa->y /= pa->w;
		pa->z /= pa->w;
		pa->w = 1;

		x = (pa->x + 1.0) * c->target->w * 0.5;
		y = (pa->y + 1.0) * c->target->h * 0.5;

		pe_setpos(x, y);

		for (j = 0; j < 3; j++) {
			int idx;
			// Traverse each vertex of triangle
			idx = pidx[(j + 1) % 3];

			pa = dbuf_get(c->vertex, idx - 1);
			pa->w = 1.0;

			mat4vec(&tmp, &c->worldmat, pa);
			pa = mat4vec(&tmp, &c->perspmat, &tmp);

			pa->x /= pa->w;
			pa->y /= pa->w;
			pa->z /= pa->w;
			pa->w = 1;

			x = (pa->x + 1.0) * c->target->w * 0.5;
			y = (pa->y + 1.0) * c->target->h * 0.5;

			pe_lineto(c->target, x, y, &(c->mat->color));
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

