#include "macros.h"
#include "render.h"
#include "surface.h"

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
	c->conf.wired = 0;

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
stroke_triangle(const struct pe_context *c, struct vec4 t[3])
{
	int i;
	
	for (i = 0; i <= 3; i++) {
		int x, y;

		x = (t[i % 3].x + 1.0) * c->target->w * 0.5;
		y = (t[i % 3].y + 1.0) * c->target->h * 0.5;

		if (i == 0)
			pe_setpos(x, y);
		else
			pe_lineto(c->target, x, y, &(c->mat->color));
	}
}

static double
rotate(double x0, double y0, double x1, double y1, double x2, double y2)
{
	return ((x2 - x1) * (y0 - y2) - (y2 - y1) * (x0 - x2));
}

void
fill_triangle(const struct pe_context *ctx, struct vec4 t[3])
{
	int x1, x2, x3, y1, y2, y3;
	int x0, y0;
	int miny, maxy;
	int minx, maxx;

	x1 = (t[0].x + 1.0) * ctx->target->w * 0.5;
	x2 = (t[1].x + 1.0) * ctx->target->w * 0.5;
	x3 = (t[2].x + 1.0) * ctx->target->w * 0.5;
	y1 = (t[0].y + 1.0) * ctx->target->h * 0.5;
	y2 = (t[1].y + 1.0) * ctx->target->h * 0.5;
	y3 = (t[2].y + 1.0) * ctx->target->h * 0.5;

	miny = MIN(y1, MIN(y2, y3));
	maxy = MAX(y1, MAX(y2, y3));
	minx = MIN(x1, MIN(x2, x3));
	maxx = MAX(x1, MAX(x2, x3));

	for (x0 = minx; x0 <= maxx; x0++) {
		for (y0 = miny; y0 <= maxy; y0++) {
			double s1, s2, s3;

			s1 = rotate(x0, y0, x1, y1, x2, y2);
			s2 = rotate(x0, y0, x2, y2, x3, y3);
			s3 = rotate(x0, y0, x3, y3, x1, y1);

			if ((s1 >= 0 && s2 >= 0 && s3 >= 0)
				|| (s1 < 0 && s2 < 0 && s3 < 0))
				pe_setpoint(ctx->target, x0, y0,
					&(ctx->mat->color));
		}
	}
}

static inline int
isbackface(struct vec4 t[3])
{
	struct vec3 viewdir = {.arr = {0, 0, -1}};
	struct vec3 a, b, c;
	double r;

	vec3sub(&a, (struct vec3 *)&t[1], (struct vec3 *)&t[0]);
	vec3sub(&b, (struct vec3 *)&t[2], (struct vec3 *)&t[0]);
	vec3cross(&c, &a, &b);

	r = vec3dot(&c, &viewdir);

	return (r > 0);
}

static inline void
draw_triangle(const struct pe_context *c, struct vec4 t[3])
{
	int i;

	for (i = 0; i < 3; i++)
		if (t[i].z < -1. || t[i].z > 1.)
			return;

	if (isbackface(t))
		return;

	if (!(c->conf.wired))
		fill_triangle(c, t);
	else
		stroke_triangle(c, t);
}

int
pe_render(struct pe_context *c)
{
	int i, j;
	struct mat4 res, prj;

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
