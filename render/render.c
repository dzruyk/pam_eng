#include <string.h>
#include <math.h>

#include "macros.h"
#include "render.h"
#include "surface.h"
#include "mesh.h"

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
	mat4identity(&c->viewportmat);

	c->mat = &pe_defmat;
	c->texture = NULL;

	c->vertex = NULL;
	c->index = NULL;

	return 0;
}

int
pe_settarget(struct pe_context *c, struct pe_surface *sur)
{
	double s;
	struct mat4 tmpm;

	if (c == NULL || sur == NULL)
		return (-1);

	c->target = sur;

	s = MIN(c->target->w, c->target->h) * 0.5;

	mat4scale(&tmpm, s, s, 1.0);
	mat4move(&(c->viewportmat), 1.0, 1.0, 0.0);
	mat4mult(&(c->viewportmat), &tmpm, &(c->viewportmat));

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

int pe_setnorm(struct pe_context *c, const struct dbuf *norm)
{
	if (c == NULL || norm == NULL)
		return (-1);

	c->norm = norm;

	return 0;
}

int pe_settexcoord(struct pe_context *c, const struct dbuf *texcoord)
{
	if (c == NULL || texcoord == NULL)
		return (-1);

	c->texcoord = texcoord;

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
stroke_triangle(const struct pe_context *c, const struct vec4 t[3])
{
	int i;

	for (i = 0; i <= 3; i++) {
		int x, y;

		x = t[i % 3].x;
		y = t[i % 3].y;

		if (i == 0)
			pe_setpos(x, y);
		else
			pe_lineto(c->target, x, y, &(c->mat->color));
	}
}

static double pointdist(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

static double interpz(double z1, double z2, double t)
{
	return (1.0 / ((1.0 - t) / z1 + t / z2));
}

static double interpattr(double a1, double z1, double a2, double z2, double t)
{
	return ((1.0 - t) * a1 / z1 + t * a2 / z2);
}

int
zbuf_cmp(const struct pe_context *c, int x, int y,
			int x1, int y1, double z1,
			int x2, int y2, double z2,
			int x3, int y3, double z3, double *zbuf)
{
	int xa, xb;
	double z, za, zb;

	if (y > y2 && y2 != y1) {
		xa = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		za = z1 + (z2 - z1) * (y - y1) / (y2 - y1);
	}
	else if (y3 != y2){
		xa = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
		za = z2 + (z3 - z2) * (y - y2) / (y3 - y2);
	}

	xb = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
	zb = z1 + (z3 - z1) * (y - y1) / (y3 - y1);

	z = za + (zb - za) * (x - xa) / (xb - xa);

	if (z < zbuf[c->target->w * y + x]) {
		zbuf[c->target->w * y + x] = z;
		return 1;
	}

	return 0;
}

struct pe_color *
zbuf_color(const struct pe_context *ctx, double *zbuf, struct pe_color *c, int x, int y)
{
	c->r = c->g = c->b = 1.0;
	c->a = zbuf[ctx->target->w * y + x] / 10.0;

	return c;
}

void
fill_triangle(const struct pe_context *ctx,
	const struct vec4 tr[3], const struct vec3 tex[3], double *zbuf)
{
	int x1, x2, x3;
	int y1, y2, y3;
	int x, y;
	double z1, z2, z3;
	double u1, u2, u3;
	double v1, v2, v3;
	double b1, b2, b3;
	struct pe_color col;

	x1 = tr[0].x;	y1 = tr[0].y;
		z1 = tr[0].z;	u1 = tex[0].x;	v1 = tex[0].y;
	x2 = tr[1].x;	y2 = tr[1].y;
		z2 = tr[1].z;	u2 = tex[1].x;	v2 = tex[1].y;
	x3 = tr[2].x;	y3 = tr[2].y;
		z3 = tr[2].z;	u3 = tex[2].x;	v3 = tex[2].y;

	if (y1 > y2) {
		swap(x1, x2);
		swap(y1, y2);
		swap(z1, z2);
		swap(u1, u2);
		swap(v1, v2);
	}

	if (y2 > y3) {
		swap(x2, x3);
		swap(y2, y3);
		swap(z2, z3);
		swap(u2, u3);
		swap(v2, v3);
	}

	if (y1 > y2) {
		swap(x1, x2);
		swap(y1, y2);
		swap(z1, z2);
		swap(u1, u2);
		swap(v1, v2);
	}

	b1 = (y2 - y1) ? ((double) (x2 - x1) / (y2 - y1)) : 0.0;
	b2 = (y3 - y1) ? ((double) (x3 - x1) / (y3 - y1)) : 0.0;
	b3 = (y3 - y2) ? ((double) (x3 - x2) / (y3 - y2)) : 0.0;

	for (y = y1; y < y3; ++y) {
		int minx, maxx;

		double us0, vs0, zs0;
		double us1, vs1, zs1;
		double u, v, z;
		double t;

		t = pointdist(x1, y1, maxx, y)
			/ pointdist(x1, y1, x3, y3);
		zs1 = interpz(z1, z3, t);
		us1 = zs1 * interpattr(u1, z1, u3, z3, t);
		vs1 = zs1 * interpattr(v1, z1, v3, z3, t);

		if (y < y2) {
			minx = (y - y1) * b1 + x1;
			maxx = (y - y1) * b2 + x1;

			t = pointdist(x1, y1, minx, y)
				/ pointdist(x1, y1, x2, y2);

			zs0 = interpz(z1, z2, t);
			us0 = zs0 * interpattr(u1, z1, u2, z2, t);
			vs0 = zs0 * interpattr(v1, z1, v2, z2, t);
		}
		else {
			minx = (y - y2) * b3 + x2;
			maxx = (y - y1) * b2 + x1;

			t = pointdist(x2, y2, minx, y)
				/ pointdist(x2, y2, x3, y3);

			zs0 = interpz(z2, z3, t);
			us0 = zs0 * interpattr(u2, z2, u3, z3, t);
			vs0 = zs0 * interpattr(v2, z2, v3, z3, t);
		}

		if (minx > maxx) {
			swap(minx, maxx);
			swap(zs0, zs1);
			swap(us0, us1);
			swap(vs0, vs1);
		}

		for (x = minx; x < maxx; ++x) {
			t = (double) (x - minx) / (maxx - minx);

			z = interpz(zs0, zs1, t);
			u = z * interpattr(us0, zs0, us1, zs1, t);
			v = z * interpattr(vs0, zs0, vs1, zs1, t);

			if (zbuf_cmp(ctx, x, y,
				x1, y1, z1, x2, y2, z2, x3, y3, z3, zbuf))
				pe_setpoint(ctx->target, x, y,
						zbuf_color(ctx, zbuf, &col, x, y));
		}
	}
}

/*
// Slower, but more stable algorithm for triangle filling
//

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

	x1 = t[0].x;	y1 = t[0].y;
	x2 = t[1].x;	y2 = t[1].y;
	x3 = t[2].x;	y3 = t[2].y;

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
*/

static inline int
isbackface(const struct vec4 t[3])
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
draw_triangle(const struct pe_context *c,
	const struct vec4 t[3], const struct vec3 tex[3], double *zbuf)
{
	int i;

	for (i = 0; i < 3; i++)
		if (t[i].z < -1. || t[i].z > 1.)
			return;

	if (isbackface(t))
		return;

	if (!(c->conf.wired))
		fill_triangle(c, t, tex, zbuf);
	else
		stroke_triangle(c, t);
}

double*
zbuf_init(struct pe_context *c)
{
	double *a;
	int i, j;

	a = malloc(c->target->w * c->target->h * sizeof(double));

	if (!a)
		return NULL;

	for (i = 0; i < c->target->h; i++)
		for (j = 0; j < c->target->w; j++)
			a[i * c->target->w + j] = 10000000.1; //SEEMS LIKE HARDCODING :D

	return a;
}

int
pe_render(struct pe_context *c)
{
	int i, j;
	struct mat4 res, prj, vp;

	double *zbuf = zbuf_init(c);

	if (!zbuf)
		return 1;
		//TODO: ERRMSG

	mat4transpose(&prj, &c->perspmat);
	mat4transpose(&res, &c->worldmat);
	mat4transpose(&vp, &c->viewportmat);
	mat4mult(&res, &res, &prj);
	mat4mult(&res, &res, &vp);
	mat4transpose(&res, &res);

	for (i = 0; i < c->index->length; i += 3) {
		struct pe_vidx *pidx;
		struct vec4 *pa, triangle[3];
		struct vec3 tex[3];

		pidx = dbuf_get(c->index, i);
		for (j = 0; j < 3; j++) {
			pa = dbuf_get(c->vertex, pidx[j].v);
			pa->w = 1;

			pa = mat4vec(&triangle[j], &res, pa);

			pa->x /= pa->w;
			pa->y /= pa->w;
			pa->z /= pa->w;
			pa->w = 1;

			memcpy(tex + j, dbuf_get(c->texcoord, pidx[j].t),
				sizeof(struct vec3));
		}

		draw_triangle(c, triangle, tex, zbuf);
	}

	free(zbuf);

	return 0;
}
