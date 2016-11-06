#ifndef _SHAPES_H__
#define _SHAPES_H__


struct vec2 {
	double x;
	double y;
};

struct vec3 {
	double x;
	double y;
	double z;
};

struct vec4 {
	double x;
	double y;
	double z;
	double w;
};

struct mat2 {
	double _11,_12,
  	       _21,_22;
};

struct mat3 {
	double _11,_12,_13,
	       _21,_22,_23,
	       _31,_32,_33;
};

struct mat4 {
	double _11,_12,_13,_14,
	       _21,_22,_23,_24,
	       _31,_32,_33,_34,
	       _41,_42,_43,_44;
};

struct vec3int {
	int x;
	int y;
	int z;
};

struct triangle {
	struct vec3 points[3];
};

struct mesh {
	struct vec3 *p;
	int *idx;
};


#endif
