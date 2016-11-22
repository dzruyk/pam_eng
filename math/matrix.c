#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dbuf.h"
#include "macros.h"
#include "matrix.h"

struct mat2 *
mat2init(struct mat2 *res, const double *arr)
{
	memcpy(res->arr, arr, sizeof(res->arr));

	return res;
}

struct mat2 *
mat2mult(struct mat2 *res, const struct mat2 *a, const struct mat2 *b)
{
	struct mat2 m;

	m._11 = a->_11 * b->_11 + a->_12 * b->_21;
	m._12 = a->_11 * b->_12 + a->_12 * b->_22;
	m._21 = a->_21 * b->_11 + a->_22 * b->_21;
	m._12 = a->_21 * b->_12 + a->_22 * b->_22;

	memcpy(res, &m, sizeof(m));

	return res;
}

struct vec2 *
mat2vec(struct vec2 *res, const struct mat2 *a, const struct vec2 *b)
{
	struct vec2 c;

	c.x = a->_11 * b->x + a->_12 * b->y;
	c.y = a->_21 * b->x + a->_22 * b->y;

	memcpy(res, &c, sizeof(c));

	return res;
}

struct mat2 *
mat2identity(struct mat2 *res)
{
	res->_11 = 1.0; res->_12 = 0.0;
	res->_21 = 0.0; res->_22 = 1.0;

	return res;
}

struct mat3 *
mat3init(struct mat3 *res, const double *arr)
{
	memcpy(res->arr, arr, sizeof(res->arr));

	return res;
}

struct mat3 *
mat3mult(struct mat3 *res, const struct mat3 *a, const struct mat3 *b)
{
	struct mat3 m;

	m._11 = a->_11 * b->_11 + a->_12 * b->_21 + a->_13 * b->_31;
	m._12 = a->_11 * b->_12 + a->_12 * b->_22 + a->_13 * b->_32;
	m._13 = a->_11 * b->_13 + a->_12 * b->_23 + a->_13 * b->_33;
	m._21 = a->_21 * b->_11 + a->_22 * b->_21 + a->_23 * b->_31;
	m._22 = a->_21 * b->_12 + a->_22 * b->_22 + a->_23 * b->_32;
	m._23 = a->_21 * b->_13 + a->_22 * b->_23 + a->_23 * b->_33;
	m._31 = a->_31 * b->_11 + a->_32 * b->_21 + a->_33 * b->_31;
	m._32 = a->_31 * b->_12 + a->_32 * b->_22 + a->_33 * b->_32;
	m._33 = a->_31 * b->_13 + a->_32 * b->_23 + a->_33 * b->_33;

	memcpy(res, &m, sizeof(m));

	return res;
}

struct vec3 *
mat3vec(struct vec3 *res, const struct mat3 *a, const struct vec3 *b)
{
	struct vec3 c;

	c.x = a->_11 * b->x + a->_12 * b->y + a->_13 * b->z;
	c.y = a->_21 * b->x + a->_22 * b->y + a->_23 * b->z;
	c.z = a->_31 * b->x + a->_32 * b->y + a->_33 * b->z;

	memcpy(res, &c, sizeof(c));

	return res;
}

struct mat3 *
mat3identity(struct mat3 *res)
{
	res->_11 = 1.0; res->_12 = 0.0; res->_13 = 0.0;
	res->_21 = 0.0; res->_22 = 1.0; res->_23 = 0.0;
	res->_31 = 0.0; res->_32 = 0.0; res->_33 = 1.0;

	return res;
}

struct mat3 *
mat3rotate(struct mat3 *res, double angle, dimension_t dimension)
{
	double s = sin(angle), c = cos(angle);

	switch (dimension) {
	case DIM_X:
		res->_11 =  1; res->_12 =  0; res->_13 =  0;
		res->_21 =  0; res->_22 =  c; res->_23 = -s;
		res->_31 =  0; res->_32 =  s; res->_33 =  c;
		break;
	case DIM_Y:
		res->_11 =  c; res->_12 =  0; res->_13 =  s;
		res->_21 =  0; res->_22 =  1; res->_23 =  0;
		res->_31 = -s; res->_32 =  0; res->_33 =  c;
		break;
	case DIM_Z:
		res->_11 =  c; res->_12 = -s; res->_13 =  0;
		res->_21 =  s; res->_22 =  c; res->_23 =  0;
		res->_31 =  0; res->_32 =  0; res->_33 =  1;
		break;
	default:
		SHOULDNT_REACH();
	}

	return res;
}

struct mat3 *
mat3scale(struct mat3 *res, double x, double y)
{
	res->_11 = x; res->_12 = 0; res->_13 = 0;
	res->_21 = 0; res->_22 = y; res->_23 = 0;
	res->_31 = 0; res->_32 = 0; res->_33 = 1;

	return res;
}

struct mat3 *
mat3move(struct mat3 *res, double x, double y)
{
	res->_11 = 1; res->_12 = 0; res->_13 = x;
	res->_21 = 0; res->_22 = 1; res->_23 = y;
	res->_31 = 0; res->_32 = 0; res->_33 = 1;

	return res;
}

struct mat4 *
mat4init(struct mat4 *res, const double *arr)
{
	memcpy(res->arr, arr, sizeof(res->arr));

	return res;
}


struct mat4 *
mat4mult(struct mat4 *res, const struct mat4 *a, const struct mat4 *b)
{
	struct mat4 m;

	m._11 = a->_11 * b->_11 + a->_12 * b->_21 + a->_13 * b->_31 + a->_14 * b->_41;
	m._12 = a->_11 * b->_12 + a->_12 * b->_22 + a->_13 * b->_32 + a->_14 * b->_42;
	m._13 = a->_11 * b->_13 + a->_12 * b->_23 + a->_13 * b->_33 + a->_14 * b->_43;
	m._14 = a->_11 * b->_14 + a->_12 * b->_24 + a->_13 * b->_34 + a->_14 * b->_44;
	m._21 = a->_21 * b->_11 + a->_22 * b->_21 + a->_23 * b->_31 + a->_24 * b->_41;
	m._22 = a->_21 * b->_12 + a->_22 * b->_22 + a->_23 * b->_32 + a->_24 * b->_42;
	m._23 = a->_21 * b->_13 + a->_22 * b->_23 + a->_23 * b->_33 + a->_24 * b->_43;
	m._24 = a->_21 * b->_14 + a->_22 * b->_24 + a->_23 * b->_34 + a->_24 * b->_44;
	m._31 = a->_31 * b->_11 + a->_32 * b->_21 + a->_33 * b->_31 + a->_34 * b->_41;
	m._32 = a->_31 * b->_12 + a->_32 * b->_22 + a->_33 * b->_32 + a->_34 * b->_42;
	m._33 = a->_31 * b->_13 + a->_32 * b->_23 + a->_33 * b->_33 + a->_34 * b->_43;
	m._34 = a->_31 * b->_14 + a->_32 * b->_24 + a->_33 * b->_34 + a->_34 * b->_44;
	m._41 = a->_41 * b->_11 + a->_42 * b->_21 + a->_43 * b->_31 + a->_44 * b->_41;
	m._42 = a->_41 * b->_12 + a->_42 * b->_22 + a->_43 * b->_32 + a->_44 * b->_42;
	m._43 = a->_41 * b->_13 + a->_42 * b->_23 + a->_43 * b->_33 + a->_44 * b->_43;
	m._44 = a->_41 * b->_14 + a->_42 * b->_24 + a->_43 * b->_34 + a->_44 * b->_44;

	memcpy(res, &m, sizeof(m));

	return res;
}

struct vec4 *
mat4vec(struct vec4 *res, const struct mat4 *a, const struct vec4 *b)
{
	struct vec4 c;

	c.x = a->_11 * b->x + a->_12 * b->y + a->_13 * b->z + a->_14 * b->w;
	c.y = a->_21 * b->x + a->_22 * b->y + a->_23 * b->z + a->_24 * b->w;
	c.z = a->_31 * b->x + a->_32 * b->y + a->_33 * b->z + a->_34 * b->w;
	c.w = a->_41 * b->x + a->_42 * b->y + a->_43 * b->z + a->_44 * b->w;

	memcpy(res, &c, sizeof(c));

	return res;
}

struct mat4 *
mat4identity(struct mat4 *res)
{
	res->_11 = 1.0; res->_12 = 0.0; res->_13 = 0.0; res->_14 = 0.0;
	res->_21 = 0.0; res->_22 = 1.0; res->_23 = 0.0; res->_24 = 0.0;
	res->_31 = 0.0; res->_32 = 0.0; res->_33 = 1.0; res->_34 = 0.0;
	res->_41 = 0.0; res->_42 = 0.0; res->_43 = 0.0; res->_44 = 1.0;

	return res;
}

struct mat4 *
mat4rotate(struct mat4 *res, double angle,  dimension_t dimension)
{
	double s = sin(angle), c = cos(angle);

	switch (dimension) {
	case DIM_X:
		res->_11 = c; res->_12 = -s; res->_13 = 0; res->_14 = 0;
		res->_21 = s; res->_22 =  c; res->_23 = 0; res->_24 = 0;
		res->_31 = 0; res->_32 =  0; res->_33 = 1; res->_34 = 0;
		res->_41 = 0; res->_42 =  0; res->_43 = 0; res->_44 = 1;
		break;
	case DIM_Y:
		res->_11 = 1; res->_12 = 0; res->_13 =  0; res->_14 = 0;
		res->_21 = 0; res->_22 = c; res->_23 = -s; res->_24 = 0;
		res->_31 = 0; res->_32 = s; res->_33 =  c; res->_34 = 0;
		res->_41 = 0; res->_42 = 0; res->_43 =  0; res->_44 = 1;
		break;
	case DIM_Z:
		res->_11 =  c; res->_12 = 0; res->_13 = s; res->_14 = 0;
		res->_21 =  0; res->_22 = 1; res->_23 = 0; res->_24 = 0;
		res->_31 = -s; res->_32 = 0; res->_33 = c; res->_34 = 0;
		res->_41 =  0; res->_42 = 0; res->_43 = 0; res->_44 = 1;
		break;
	default:
		SHOULDNT_REACH();
	}

	return res;
}

struct mat4 *
mat4move(struct mat4 *res, double x, double y, double z)
{
	res->_11 = 1; res->_12 = 0; res->_13 = 0; res->_14 = x;
	res->_21 = 0; res->_22 = 1; res->_23 = 0; res->_24 = y;
	res->_31 = 0; res->_32 = 0; res->_33 = 1; res->_34 = z;
	res->_41 = 0; res->_42 = 0; res->_43 = 0; res->_44 = 1;

	return res;
}

struct mat4 *
mat4scale(struct mat4 *res, double x, double y, double z)
{
	res->_11 = x; res->_12 = 0; res->_13 = 0; res->_14 = 0;
	res->_21 = 0; res->_22 = y; res->_23 = 0; res->_24 = 0;
	res->_31 = 0; res->_32 = 0; res->_33 = z; res->_34 = 0;
	res->_41 = 0; res->_42 = 0; res->_43 = 0; res->_44 = 1;

	return res;
}

