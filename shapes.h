#ifndef _SHAPES_H__
#define _SHAPES_H__

struct vec3 {
	double x;
	double y;
	double z;
};

struct triangle {
	struct vec3 points[3];
};

struct mesh {
	struct vec3 *p;
	int *idx;
};


#endif
