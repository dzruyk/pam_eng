#include <string.h>
#include <math.h>

#include "macros.h"
#include "render.h"
#include "surface.h"
#include "mesh.h"

struct pe_material pe_defmat = { {1.0, 1.0, 1.0, 1.0} };

struct pe_surface sur;

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

	pe_surfromfile("african_head_diffuse.png", &sur);
//	pe_surfromfile("rooster_color.png", &sur);
//	pe_surfromfile("chesstex.png", &sur);

	mat4identity(&c->worldmat);
	mat4identity(&c->perspmat);
	mat4identity(&c->viewportmat);

	c->zbuffer = NULL;

	c->mat = &pe_defmat;
	c->texture = NULL;

	c->vertex = NULL;
	c->index = NULL;

	return 0;
}

int
pe_settarget(struct pe_context *c, struct pe_surface *sur)
{
	struct mat4 tmpm;

	if (c == NULL || sur == NULL)
		return (-1);

	c->target = sur;

	mat4scale(&tmpm, c->target->w * 0.5, c->target->h * 0.5, 1.0);
	mat4move(&(c->viewportmat), 1.0, 1.0, 0.0);
	mat4mult(&(c->viewportmat), &tmpm, &(c->viewportmat));

	if (c->zbuffer != NULL)
		free(c->zbuffer);

	c->zbuffer = malloc(c->target->w * c->target->h * sizeof(double));

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

#define set_vpoint(vp, idx) 	do {		\
	vp.x = (int)round(tr[idx].x);		\
	vp.y = (int)round(tr[idx].y);		\
	vp.z = tr[idx].z;			\
	vp.u = tex[idx].x;			\
	vp.v = tex[idx].y;			\
} while (0)

void
fill_triangle(const struct pe_context *ctx,
	const struct vec4 tr[3], const struct vec3 tex[3])
{
	int x, y;
	double b1, b2, b3;

	struct vpoint {
		int x, y;
		double z;
		double u, v;
	} p1, p2, p3;

	set_vpoint(p1, 0);
	set_vpoint(p2, 1);
	set_vpoint(p3, 2);

	if (p1.y > p2.y)
		swap(p1, p2);

	if (p2.y > p3.y)
		swap(p2, p3);

	if (p1.y > p2.y)
		swap(p1, p2);

	b1 = (p2.y - p1.y) ? ((double) (p2.x - p1.x) / (p2.y - p1.y)) : 0.0;
	b2 = (p3.y - p1.y) ? ((double) (p3.x - p1.x) / (p3.y - p1.y)) : 0.0;
	b3 = (p3.y - p2.y) ? ((double) (p3.x - p2.x) / (p3.y - p2.y)) : 0.0;

	for (y = p1.y; y < p3.y; ++y) {
		struct vpoint *pp1, *pp2, *pp3;
		double bx = b1;
		int minx, maxx;

		double us0, vs0, zs0;
		double us1, vs1, zs1;
		double u, v, z;
		double t;

		pp1 = &p1;
		pp2 = &p2;
		pp3 = &p3;

		if (y > p2.y || p1.y == p2.y) {
			swap(pp1, pp3);
			bx = b3;
		}

		minx = (y - pp1->y) * bx + pp1->x;
		maxx = (y - pp1->y) * b2 + pp1->x;

		t = pointdist(pp1->x, pp1->y, maxx, y)
			/ pointdist(pp1->x, pp1->y, pp3->x, pp3->y);

		zs1 = interpz(pp1->z, pp3->z, t);
		us1 = zs1 * interpattr(pp1->u, pp1->z, pp3->u, pp3->z, t);
		vs1 = zs1 * interpattr(pp1->v, pp1->z, pp3->v, pp3->z, t);


		t = pointdist(pp1->x, pp1->y, minx, y)
			/ pointdist(pp1->x, pp1->y, pp2->x, pp2->y);

		zs0 = interpz(pp1->z, pp2->z, t);
		us0 = zs0 * interpattr(pp1->u, pp1->z, pp2->u, pp2->z, t);
		vs0 = zs0 * interpattr(pp1->v, pp1->z, pp2->v, pp2->z, t);

		if (minx > maxx) {
			swap(minx, maxx);
			swap(zs0, zs1);
			swap(us0, us1);
			swap(vs0, vs1);
		}

		for (x = minx; x < maxx; ++x) {
			struct pe_color col;

			t = (double) (x - minx) / (double) (maxx - minx);

			z = interpz(zs0, zs1, t);

			u = z * interpattr(us0, zs0, us1, zs1, t);
			v = z * interpattr(vs0, zs0, vs1, zs1, t);

			if (y < 0 || x < 0 || y >= ctx->target->h
				|| x >= ctx->target->w)
				continue;

			if (z > 0.0
				&& z < ctx->zbuffer[y * ctx->target->w + x]) {

				pe_getpoint(&sur, u, 1.0 - v, &col);
				pe_setpoint(ctx->target, x, y, &col);

				ctx->zbuffer[y * ctx->target->w + x] = z;
			}
		}
	}
}
#undef set_vpoint
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
	const struct vec4 t[3], const struct vec3 tex[3])
{
	if (isbackface(t))
		return;

	if (!(c->conf.wired))
		fill_triangle(c, t, tex);
	else
		stroke_triangle(c, t);
}

int
pe_render(struct pe_context *c)
{
	int i, j;
	struct mat4 res;

	mat4mult(&res, &(c->perspmat), &(c->worldmat));
	mat4mult(&res, &(c->viewportmat), &res);

	for (i = 0; i < c->target->w * c->target->h; ++i)
		c->zbuffer[i] = 1.0;

	for (i = 0; i < c->index->length; i += 3) {
		struct pe_vidx *pidx;
		struct vec4 *pa, triangle[3];
		struct vec3 tex[3];

		pidx = dbuf_get(c->index, i);
		for (j = 0; j < 3; j++) {
			pa = dbuf_get(c->vertex, pidx[j].v);
			pa->w = 1.0;

			pa = mat4vec(triangle + j, &res, pa);

			pa->x /= pa->w;
			pa->y /= pa->w;
			pa->z /= pa->w;
			pa->w = 1;

			memcpy(tex + j, dbuf_get(c->texcoord, pidx[j].t),
				sizeof(struct vec3));
		}

		draw_triangle(c, triangle, tex);
	}

	return 0;
}
