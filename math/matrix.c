#include <stdlib.h>
#include <math.h>

#include "shapes.h"
#include "matrix.h"


struct mat2
mat2init(double *arr)
{
	struct mat2 m;

	m._11 = *arr++; m._12 = *arr++;
	m._21 = *arr++; m._22 = *arr++;

	return m;
}

struct mat2
mat2mult(struct mat2 a, struct mat2 b)
{
	struct mat2 m;

	m._11 = a._11 * b._11 + a._12 * b._21;
	m._12 = a._11 * b._12 + a._12 * b._22;
	m._21 = a._21 * b._11 + a._22 * b._21;
	m._12 = a._21 * b._12 + a._22 * b._22;

	return m;
}

struct vec2
mat2vec(struct mat2 a, struct vec2 b)
{
	struct vec2 c;

	c.x = a._11 * b.x + a._12 * b.y;
	c.y = a._21 * b.x + a._22 * b.y;

	return c;
}

struct mat2
mat2identity()
{
	struct mat2 m;

	m._11 = 1.0; m._12 = 0.0;
	m._21 = 0.0; m._22 = 1.0;

	return m;
}

struct mat3
mat3init(double *arr)
{
	struct mat3 m;

	m._11 = *arr++; m._12 = *arr++; m._13 = *arr++;
	m._21 = *arr++; m._22 = *arr++; m._23 = *arr++;
	m._31 = *arr++; m._32 = *arr++; m._33 = *arr++;

	return m;
}

struct mat3
mat3mult(struct mat3 a, struct mat3 b)
{
	struct mat3 m;

	m._11 = a._11 * b._11 + a._12 * b._21 + a._13 * b._31;
	m._12 = a._11 * b._12 + a._12 * b._22 + a._13 * b._32;
	m._13 = a._11 * b._13 + a._12 * b._23 + a._13 * b._33;
	m._21 = a._21 * b._11 + a._22 * b._21 + a._23 * b._31;
	m._22 = a._21 * b._12 + a._22 * b._22 + a._23 * b._32;
	m._23 = a._21 * b._13 + a._22 * b._23 + a._23 * b._33;
	m._31 = a._31 * b._11 + a._32 * b._21 + a._33 * b._31;
	m._32 = a._31 * b._12 + a._32 * b._22 + a._33 * b._32;
	m._33 = a._31 * b._13 + a._32 * b._23 + a._33 * b._33;

	return m;
}

struct vec3
mat3vec(struct mat3 a, struct vec3 b)
{
	struct vec3 c;

	c.x = a._11 * b.x + a._12 * b.y + a._13 * b.z;
	c.y = a._21 * b.x + a._22 * b.y + a._23 * b.z;
	c.z = a._31 * b.x + a._32 * b.y + a._33 * b.z;

	return c;
}

struct mat3
mat3identity()
{
	struct mat3 m;

	m._11 = 1.0; m._12 = 0.0; m._13 = 0.0;
	m._21 = 0.0; m._22 = 1.0; m._23 = 0.0;
	m._31 = 0.0; m._32 = 0.0; m._33 = 1.0;
	
	return m;
}

struct mat3
mat3rotate(double angle, unsigned char dimension)
{
	struct mat3 m;

	double s = sin(angle), c = cos(angle);

	if (!dimension) {
		m._11 =  1; m._12 =  0; m._13 =  0;
		m._21 =  0; m._22 =  c; m._23 = -s;
		m._31 =  0; m._32 =  s; m._33 =  c;
	}
	else if (dimension == 1) {
		m._11 =  c; m._12 =  0; m._13 =  s;
		m._21 =  0; m._22 =  1; m._23 =  0;
		m._31 = -s; m._32 =  0; m._33 =  c;
	}
	else {
		m._11 =  c; m._12 = -s; m._13 =  0;
		m._21 =  s; m._22 =  c; m._23 =  0;
		m._31 =  0; m._32 =  0; m._33 =  1;
	}
	return m;
}

struct mat3
mat3scale(double a, double b)
{
	struct mat3 m;

	m._11 = a; m._12 = 0; m._13 = 0;
	m._21 = 0; m._22 = b; m._23 = 0;
	m._31 = 0; m._32 = 0; m._33 = 1;

	return m;
}

struct mat3
mat3move(double a, double b)
{
	struct mat3 m;

	m._11 = 1; m._12 = 0; m._13 = a;
	m._21 = 0; m._22 = 1; m._23 = b;
	m._31 = 0; m._32 = 0; m._33 = 1;

	return m;
}

struct mat4
mat4init(double *arr)
{
	struct mat4 m;

	m._11 = *arr++; m._12 = *arr++; m._13 = *arr++; m._14 = *arr++;
	m._21 = *arr++; m._22 = *arr++; m._23 = *arr++; m._24 = *arr++;
	m._31 = *arr++; m._32 = *arr++; m._33 = *arr++; m._34 = *arr++;
	m._41 = *arr++; m._42 = *arr++; m._43 = *arr++; m._44 = *arr++;

	return m;
}


struct mat4
mat4mult(struct mat4 a, struct mat4 b)
{
	struct mat4 m;

	m._11 = a._11 * b._11 + a._12 * b._21 + a._13 * b._31 + a._14 * b._41;
	m._12 = a._11 * b._12 + a._12 * b._22 + a._13 * b._32 + a._14 * b._42;
	m._13 = a._11 * b._13 + a._12 * b._23 + a._13 * b._33 + a._14 * b._43;
	m._14 = a._11 * b._14 + a._12 * b._24 + a._13 * b._34 + a._14 * b._44;
	m._21 = a._21 * b._11 + a._22 * b._21 + a._23 * b._31 + a._24 * b._41;
	m._22 = a._21 * b._12 + a._22 * b._22 + a._23 * b._32 + a._24 * b._42;
	m._23 = a._21 * b._13 + a._22 * b._23 + a._23 * b._33 + a._24 * b._43;
	m._24 = a._21 * b._14 + a._22 * b._24 + a._23 * b._34 + a._24 * b._44;
	m._31 = a._31 * b._11 + a._32 * b._21 + a._33 * b._31 + a._34 * b._41;
	m._32 = a._31 * b._12 + a._32 * b._22 + a._33 * b._32 + a._34 * b._42;
	m._33 = a._31 * b._13 + a._32 * b._23 + a._33 * b._33 + a._34 * b._43;
	m._34 = a._31 * b._14 + a._32 * b._24 + a._33 * b._34 + a._34 * b._44;
	m._41 = a._41 * b._11 + a._42 * b._21 + a._43 * b._31 + a._44 * b._41;
	m._42 = a._41 * b._12 + a._42 * b._22 + a._43 * b._32 + a._44 * b._42;
	m._43 = a._41 * b._13 + a._42 * b._23 + a._43 * b._33 + a._44 * b._43;
	m._44 = a._41 * b._14 + a._42 * b._24 + a._43 * b._34 + a._44 * b._44;

	return m;
}

struct vec4
mat4vec(struct mat4 a, struct vec4 b)
{
	struct vec4 c;

	c.x = a._11 * b.x + a._12 * b.y + a._13 * b.z + a._14 * b.w;
	c.y = a._21 * b.x + a._22 * b.y + a._23 * b.z + a._24 * b.w;
	c.z = a._31 * b.x + a._32 * b.y + a._33 * b.z + a._34 * b.w;
	c.w = a._41 * b.x + a._42 * b.y + a._43 * b.z + a._44 * b.w;

	return c;
}

struct mat4
mat4identity()
{
	struct mat4 m;

	m._11 = 1.0; m._12 = 0.0; m._13 = 0.0; m._14 = 0.0;
	m._21 = 0.0; m._22 = 1.0; m._23 = 0.0; m._24 = 0.0;
	m._31 = 0.0; m._32 = 0.0; m._33 = 1.0; m._34 = 0.0;
	m._41 = 0.0; m._42 = 0.0; m._43 = 0.0; m._44 = 1.0;
	
	return m;
}

struct mat4
mat4rotate(double angle, unsigned char dimension)
{
	struct mat4 m;

	double s = sin(angle), c = cos(angle);

	if (!dimension) {
		m._11 = c; m._12 = -s; m._13 = 0; m._14 = 0;
		m._21 = s; m._22 =  c; m._23 = 0; m._24 = 0;
		m._31 = 0; m._32 =  0; m._33 = 1; m._34 = 0;
		m._41 = 0; m._42 =  0; m._43 = 0; m._44 = 1;
	}
	else if (dimension == 1) {
		m._11 = 1; m._12 = 0; m._13 =  0; m._14 = 0;
		m._21 = 0; m._22 = c; m._23 = -s; m._24 = 0;
		m._31 = 0; m._32 = s; m._33 =  c; m._34 = 0;
		m._41 = 0; m._42 = 0; m._43 =  0; m._44 = 1;
	}
	else {
		m._11 =  c; m._12 = 0; m._13 = s; m._14 = 0;
		m._21 =  0; m._22 = 1; m._23 = 0; m._24 = 0;
		m._31 = -s; m._32 = 0; m._33 = c; m._34 = 0;
		m._41 =  0; m._42 = 0; m._43 = 0; m._44 = 1;
	}
	return m;
}

struct mat4
mat4move(double a, double b, double c)
{
	struct mat4 m;

	m._11 = 1; m._12 = 0; m._13 = 0; m._14 = a;
	m._21 = 0; m._22 = 1; m._23 = 0; m._24 = b;
	m._31 = 0; m._32 = 0; m._33 = 1; m._34 = c;
	m._41 = 0; m._42 = 0; m._43 = 0; m._44 = 1;

	return m;
}

struct mat4
mat4scale(double a, double b, double c)
{
	struct mat4 m;

	m._11 = a; m._12 = 0; m._13 = 0; m._14 = 0;
	m._21 = 0; m._22 = b; m._23 = 0; m._24 = 0;
	m._31 = 0; m._32 = 0; m._33 = c; m._34 = 0;
	m._41 = 0; m._42 = 0; m._43 = 0; m._44 = 1;

	return m;
}


