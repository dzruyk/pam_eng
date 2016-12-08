#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef enum  {
	DIM_X = 0,
	DIM_Y = 1,
	DIM_Z = 2,
} dimension_t;

struct vec2 {
	union {
		struct {
			double x;
			double y;
		};
		double arr[2];
	};
};

struct vec3 {
	union {
		struct {
			double x;
			double y;
			double z;
		};
		double arr[3];
	};
};

struct vec4 {
	union {
		struct {
			double x;
			double y;
			double z;
			double w;
		};
		double arr[4];
	};
};

struct mat2 {
	union {
		struct {
			double _11,_12,
		  	       _21,_22;
		};
		double arr[2][2];
	};
};

struct mat3 {
	union {
		struct {
			double _11,_12,_13,
			       _21,_22,_23,
			       _31,_32,_33;
		};
		double arr[3][3];
	};
};

struct mat4 {
	union {
		struct {
			double _11,_12,_13,_14,
			       _21,_22,_23,_24,
			       _31,_32,_33,_34,
			       _41,_42,_43,_44;
		};
		double arr[4][4];
	};
};

struct vec3int {
	union {
		struct {
			int x;
			int y;
			int z;
		};
		int arr[3];
	};
};

struct mat2 *mat2identity(struct mat2 *res);
struct mat2 *mat2init(struct mat2 *res, const double *arr);
struct mat2 *mat2mult(struct mat2 *res, const struct mat2 *a, const struct mat2 *b);
struct vec2 *mat2vec(struct vec2 *res, const struct mat2 *a, const struct vec2 *b);
struct mat2 *mat2transpose(struct mat2 *res, const struct mat2 *src);

struct mat3 *mat3identity(struct mat3 *res);
struct mat3 *mat3init(struct mat3 *res, const double *arr);
struct mat3 *mat3mult(struct mat3 *res, const struct mat3 *a, const struct mat3 *b);
struct vec3 *mat3vec(struct vec3 *res, const struct mat3 *a, const struct vec3 *b);
struct mat3 *mat3rotate(struct mat3 *res, double angle, dimension_t dimension);
struct mat3 *mat3scale(struct mat3 *res, double x, double y);
struct mat3 *mat3transpose(struct mat3 *res, const struct mat3 *src);
struct mat3 *mat3move(struct mat3 *res, double x, double y);

struct mat4 *mat4identity(struct mat4 *res);
struct mat4 *mat4init(struct mat4 *res, const double *arr);
struct mat4 *mat4mult(struct mat4 *res, const struct mat4 *a, const struct mat4 *b);
struct vec4 *mat4vec(struct vec4 *res, const struct mat4 *a, const struct vec4 *b);
struct mat4 *mat4rotate(struct mat4 *res, double angle,  dimension_t dimension);
struct mat4 *mat4move(struct mat4 *res, double x, double y, double z);
struct mat4 *mat4scale(struct mat4 *res, double x, double y, double z);
struct mat4 *mat4transpose(struct mat4 *res, const struct mat4 *src);
struct mat4 *mat4persp(struct mat4 *res, double near, double far, double left, double right, double bot, double top);

struct vec3 *vec3add(struct vec3 *res, const struct vec3 *a, const struct vec3 *b);
struct vec3 *vec3sub(struct vec3 *res, const struct vec3 *a, const struct vec3 *b);
double vec3dot(const struct vec3 *a, const struct vec3 *b);
struct vec3 *vec3cross(struct vec3 *res, const struct vec3 *a, const struct vec3 *b);
struct vec3 *vec3norm(struct vec3 *res, const struct vec3 *src);
#endif
