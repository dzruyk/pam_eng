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

static void
draw_triangle(const struct pe_context *ctx, struct vec4 t[3])
{
	int i;
	struct vec3 a, b, c;
	struct vec3 viewdir = {0, 0, -1};
	double r;

	for (i = 0; i < 3; i++) {
		int x, y;

		x = (t[i].x + 1.0) * ctx->target->w * 0.5;
		y = (t[i].y + 1.0) * ctx->target->h * 0.5;

		if (i == 0)
			pe_setpos(x, y);
		else
			pe_lineto(ctx->target, x, y, &(ctx->mat->color));
	}
}

static int
wiredrender(const struct pe_context *c)
{
	int i, j;
	struct mat4 res, prj;

	pe_setperspmatrix((struct pe_context *)c, mat4persp(&prj, 1, 100, -1, 1, -1, 1));
	mat4transpose(&prj, &c->perspmat);
	mat4transpose(&res, &c->worldmat);
	mat4mult(&res, &res, &prj);
	mat4transpose(&res, &res);

	for (i = 0; i < c->index->length; i += 3) {
		int *pidx;
		struct vec4 *pa, triangle[3];

		pidx = dbuf_get(c->index, i);
		for (j = 0; j < 3; j++) {
			pa = dbuf_get(c->vertex, pidx[j]);
			pa->w = 1;

			pa = mat4vec(&triangle[j], &res, pa);

			pa->x /= pa->w;
			pa->y /= pa->w;
			pa->z /= pa->w;
			pa->w = 1;
		}

		draw_triangle(c, triangle);
	}

	return 0;
}

int
pe_render(struct pe_context *c)
{
	wiredrender(c);

	return 0;
}

