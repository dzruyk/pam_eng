#ifndef _MATRIX_H_
#define _MATRIX_H_


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


struct mat2 mat2init(double *arr);

struct mat2 mat2mult(struct mat2 a, struct mat2 b);

struct vec2 mat2vec(struct mat2 a, struct vec2 b);

struct mat2 mat2identity();

struct mat3 mat3init(double *arr);

struct mat3 mat3mult(struct mat3 a, struct mat3 b);

struct vec3 mat3vec(struct mat3 a, struct vec3 b);

struct mat3 mat3identity();

struct mat3 mat3rotate(double angle, unsigned char dimension);

struct mat3 mat3scale(double a, double b);

struct mat3 mat3move(double a, double b);

struct mat4 mat4init(double *arr);

struct mat4 mat4mult(struct mat4 a, struct mat4 b);

struct vec4 mat4vec(struct mat4 a, struct vec4 b);

struct mat4 mat4identity();

struct mat4 mat4rotate(double angle, unsigned char dimension);

struct mat4 mat4scale(double a, double b, double c);

struct mat4 mat4move(double a, double b, double c);

#endif
