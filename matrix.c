#include <stdlib.h>
#include <math.h>

#include "shapes.h"
#include "matrix.h"


struct mat2 *
mat2init(double *arr)
{
	struct mat2 *matrix = malloc(sizeof(struct mat2));

	matrix->_11 = *arr++;
	matrix->_12 = *arr++;
	matrix->_21 = *arr++;
	matrix->_22 = *arr++;

	return matrix;
}

struct mat2 *
mat2mult(struct mat2 *a, struct mat2 *b)
{
	struct mat2 *matrix = malloc(sizeof(struct mat2));

	matrix->_11 = a->_11 * b->_11 + a->_12 * b->_21;
	matrix->_12 = a->_11 * b->_12 + a->_12 * b->_22;
	matrix->_21 = a->_21 * b->_11 + a->_22 * b->_21;
	matrix->_12 = a->_21 * b->_12 + a->_22 * b->_22;

	return matrix;
}

void
mat2vec(struct mat2 *a, struct vec2 *b)
{
	a->_11 = a->_11 * b->x + a->_12 * b->y;
	a->_12 = a->_11 * b->x + a->_12 * b->y;
	a->_21 = a->_21 * b->x + a->_22 * b->y;
	a->_12 = a->_21 * b->x + a->_22 * b->y;
}

struct mat3 *
mat3init(double *arr)
{
	struct mat3 *matrix = malloc(sizeof(struct mat3));

	matrix->_11 = *arr++;
	matrix->_12 = *arr++;
	matrix->_13 = *arr++;
	matrix->_21 = *arr++;
	matrix->_22 = *arr++;
	matrix->_23 = *arr++;
	matrix->_31 = *arr++;
	matrix->_32 = *arr++;
	matrix->_33 = *arr++;

	return matrix;
}

struct mat3 *
mat3mult(struct mat3 *a, struct mat3 *b)
{
	struct mat3 *matrix = malloc(sizeof(struct mat3));

	matrix->_11 = a->_11 * b->_11 + a->_12 * b->_21 + a->_13 * b->_31;
	matrix->_12 = a->_11 * b->_12 + a->_12 * b->_22 + a->_13 * b->_32;
	matrix->_13 = a->_11 * b->_13 + a->_12 * b->_23 + a->_13 * b->_33;
	matrix->_21 = a->_21 * b->_11 + a->_22 * b->_21 + a->_23 * b->_31;
	matrix->_22 = a->_21 * b->_12 + a->_22 * b->_22 + a->_23 * b->_32;
	matrix->_23 = a->_21 * b->_13 + a->_22 * b->_23 + a->_23 * b->_33;
	matrix->_31 = a->_31 * b->_11 + a->_32 * b->_21 + a->_33 * b->_31;
	matrix->_32 = a->_31 * b->_12 + a->_32 * b->_22 + a->_33 * b->_32;
	matrix->_33 = a->_31 * b->_13 + a->_32 * b->_23 + a->_33 * b->_33;

	return matrix;
}

struct vec3 *
mat3vec(struct mat3 *a, struct vec3 *b)
{
	struct vec3 *c = malloc(sizeof(struct vec3));

	if (!c)
		return NULL;

	c->x = a->_11 * b->x + a->_12 * b->y + a->_13 * b->z;
	c->y = a->_21 * b->x + a->_22 * b->y + a->_23 * b->z;
	c->z = a->_31 * b->x + a->_32 * b->y + a->_33 * b->z;

	return c;
}

struct mat3 *
mat3rotate(double angle, unsigned char dimension)
{
	struct mat3 *m = malloc(sizeof(struct mat3));

	if (!m)
		return NULL;

	double s = sin(angle), c = cos(angle);

	if (!dimension) {
		m->_11 =  1; m->_12 =  0; m->_13 =  0;
		m->_21 =  0; m->_22 =  c; m->_23 = -s;
		m->_31 =  0; m->_32 =  s; m->_33 =  c;
	}
	else if (dimension == 1) {
		m->_11 =  c; m->_12 =  0; m->_13 =  s;
		m->_21 =  0; m->_22 =  1; m->_23 =  0;
		m->_31 = -s; m->_32 =  0; m->_33 =  c;
	}
	else {
		m->_11 =  c; m->_12 = -s; m->_13 =  0;
		m->_21 =  s; m->_22 =  c; m->_23 =  0;
		m->_31 =  0; m->_32 =  0; m->_33 =  1;
	}
	return m;
}

struct mat3 *
mat3scale(double a, double b)
{
	struct mat3 *m = malloc(sizeof(struct mat3));

	m->_11 = a; m->_12 = 0; m->_13 = 0;
	m->_21 = 0; m->_22 = b; m->_23 = 0;
	m->_31 = 0; m->_32 = 0; m->_33 = 1;
	return m;
}

struct mat3 *
mat3move(double a, double b)
{
	struct mat3 *m = malloc(sizeof(struct mat3));

	m->_11 = 1; m->_12 = 0; m->_13 = a;
	m->_21 = 0; m->_22 = 1; m->_23 = b;
	m->_31 = 0; m->_32 = 0; m->_33 = 1;
	return m;
}

struct mat4 *
mat4init(double *arr)
{
	struct mat4 *matrix = malloc(sizeof(struct mat4));

	matrix->_11 = *arr++; matrix->_12 = *arr++; matrix->_13 = *arr++; matrix->_14 = *arr++;
	matrix->_21 = *arr++; matrix->_22 = *arr++; matrix->_23 = *arr++; matrix->_24 = *arr++;
	matrix->_31 = *arr++; matrix->_32 = *arr++; matrix->_33 = *arr++; matrix->_34 = *arr++;
	matrix->_41 = *arr++; matrix->_42 = *arr++; matrix->_43 = *arr++; matrix->_44 = *arr++;

	return matrix;
}

struct mat4 *
mat4mult(struct mat4 *a, struct mat4 *b)
{
	struct mat4 *matrix = malloc(sizeof(struct mat4));

	matrix->_11 = a->_11 * b->_11 + a->_12 * b->_21 + a->_13 * b->_31 + a->_14 * b->_41;
	matrix->_12 = a->_11 * b->_12 + a->_12 * b->_22 + a->_13 * b->_32 + a->_14 * b->_42;
	matrix->_13 = a->_11 * b->_13 + a->_12 * b->_23 + a->_13 * b->_33 + a->_14 * b->_43;
	matrix->_14 = a->_11 * b->_14 + a->_12 * b->_24 + a->_13 * b->_34 + a->_14 * b->_44;
	matrix->_21 = a->_21 * b->_11 + a->_22 * b->_21 + a->_23 * b->_31 + a->_24 * b->_41;
	matrix->_22 = a->_21 * b->_12 + a->_22 * b->_22 + a->_23 * b->_32 + a->_24 * b->_42;
	matrix->_23 = a->_21 * b->_13 + a->_22 * b->_23 + a->_23 * b->_33 + a->_24 * b->_43;
	matrix->_24 = a->_21 * b->_14 + a->_22 * b->_24 + a->_23 * b->_34 + a->_24 * b->_44;
	matrix->_31 = a->_31 * b->_11 + a->_32 * b->_21 + a->_33 * b->_31 + a->_34 * b->_41;
	matrix->_32 = a->_31 * b->_12 + a->_32 * b->_22 + a->_33 * b->_32 + a->_34 * b->_42;
	matrix->_33 = a->_31 * b->_13 + a->_32 * b->_23 + a->_33 * b->_33 + a->_34 * b->_43;
	matrix->_34 = a->_31 * b->_14 + a->_32 * b->_24 + a->_33 * b->_34 + a->_34 * b->_44;
	matrix->_41 = a->_41 * b->_11 + a->_42 * b->_21 + a->_43 * b->_31 + a->_44 * b->_41;
	matrix->_42 = a->_41 * b->_12 + a->_42 * b->_22 + a->_43 * b->_32 + a->_44 * b->_42;
	matrix->_43 = a->_41 * b->_13 + a->_42 * b->_23 + a->_43 * b->_33 + a->_44 * b->_43;
	matrix->_44 = a->_41 * b->_14 + a->_42 * b->_24 + a->_43 * b->_34 + a->_44 * b->_44;

	return matrix;
}

void
mat4vec(struct mat4 *a, struct vec4 *b)
{
	a->_11 = a->_11 * b->x + a->_12 * b->y + a->_13 * b->z + a->_14 * b->w;
	a->_12 = a->_11 * b->x + a->_12 * b->y + a->_13 * b->z + a->_14 * b->w;
	a->_13 = a->_11 * b->x + a->_12 * b->y + a->_13 * b->z + a->_14 * b->w;
	a->_14 = a->_11 * b->x + a->_12 * b->y + a->_13 * b->z + a->_14 * b->w;
	a->_21 = a->_21 * b->x + a->_22 * b->y + a->_23 * b->z + a->_24 * b->w;
	a->_22 = a->_21 * b->x + a->_22 * b->y + a->_23 * b->z + a->_24 * b->w;
	a->_23 = a->_21 * b->x + a->_22 * b->y + a->_23 * b->z + a->_24 * b->w;
	a->_24 = a->_21 * b->x + a->_22 * b->y + a->_23 * b->z + a->_24 * b->w;
	a->_31 = a->_31 * b->x + a->_32 * b->y + a->_33 * b->z + a->_34 * b->w;
	a->_32 = a->_31 * b->x + a->_32 * b->y + a->_33 * b->z + a->_34 * b->w;
	a->_33 = a->_31 * b->x + a->_32 * b->y + a->_33 * b->z + a->_34 * b->w;
	a->_34 = a->_31 * b->x + a->_32 * b->y + a->_33 * b->z + a->_34 * b->w;
	a->_41 = a->_41 * b->x + a->_42 * b->y + a->_43 * b->z + a->_44 * b->w;
	a->_42 = a->_41 * b->x + a->_42 * b->y + a->_43 * b->z + a->_44 * b->w;
	a->_43 = a->_41 * b->x + a->_42 * b->y + a->_43 * b->z + a->_44 * b->w;
	a->_44 = a->_41 * b->x + a->_42 * b->y + a->_43 * b->z + a->_44 * b->w;
}

struct mat4 *
mat4rotate(double angle, unsigned char dimension)
{
	struct mat4 *m = malloc(sizeof(struct mat4));

	double s = sin(angle), c = cos(angle);

	if (!dimension) {
		m->_11 = c; m->_12 = -s; m->_13 = 0; m->_14 = 0;
		m->_21 = s; m->_22 =  c; m->_23 = 0; m->_24 = 0;
		m->_31 = 0; m->_32 =  0; m->_33 = 1; m->_34 = 0;
		m->_41 = 0; m->_42 =  0; m->_43 = 0; m->_44 = 1;
	}
	else if (dimension == 1) {
		m->_11 = 1; m->_12 = 0; m->_13 =  0; m->_14 = 0;
		m->_21 = 0; m->_22 = c; m->_23 = -s; m->_24 = 0;
		m->_31 = 0; m->_32 = s; m->_33 =  c; m->_34 = 0;
		m->_41 = 0; m->_42 = 0; m->_43 =  0; m->_44 = 1;
	}
	else {
		m->_11 =  c; m->_12 = 0; m->_13 = s; m->_14 = 0;
		m->_21 =  0; m->_22 = 1; m->_23 = 0; m->_24 = 0;
		m->_31 = -s; m->_32 = 0; m->_33 = c; m->_34 = 0;
		m->_41 =  0; m->_42 = 0; m->_43 = 0; m->_44 = 1;
	}
	return m;
}


struct mat4 *
mat4move(double a, double b, double c)
{
	struct mat4 *m = malloc(sizeof(struct mat4));

	m->_11 = 1; m->_12 = 0; m->_13 = 0; m->_14 = a;
	m->_21 = 0; m->_22 = 1; m->_23 = 0; m->_24 = b;
	m->_31 = 0; m->_32 = 0; m->_33 = 1; m->_34 = c;
	m->_41 = 0; m->_42 = 0; m->_43 = 0; m->_44 = 1;

	return m;
}

struct mat4 *
mat4scale(double a, double b, double c)
{
	struct mat4 *m = malloc(sizeof(struct mat4));

	m->_11 = a; m->_12 = 0; m->_13 = 0; m->_14 = 0;
	m->_21 = 0; m->_22 = b; m->_23 = 0; m->_24 = 0;
	m->_31 = 0; m->_32 = 0; m->_33 = c; m->_34 = 0;
	m->_41 = 0; m->_42 = 0; m->_43 = 0; m->_44 = 1;

	return m;
}


