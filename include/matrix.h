#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "shapes.h"


struct mat2
mat2init(double *arr);

struct mat2
mat2mult(struct mat2 a, struct mat2 b);

struct vec2
mat2vec(struct mat2 a, struct vec2 b);

struct mat3
mat3init(double *arr);

struct mat3
mat3mult(struct mat3 a, struct mat3 b);

struct vec3
mat3vec(struct mat3 a, struct vec3 b);

struct mat3
mat3rotate(double angle, unsigned char dimension);

struct mat3 
mat3scale(double a, double b);

struct mat3 
mat3move(double a, double b);

struct mat4 
mat4init(double *arr);

struct mat4 
mat4mult(struct mat4 a, struct mat4 b);

struct mat4 
mat4rotate(double angle, unsigned char dimension);

struct mat4 
mat4scale(double a, double b, double c);

struct mat4 
mat4move(double a, double b, double c);


#endif
