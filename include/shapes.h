#ifndef _SHAPES_H__
#define _SHAPES_H__

#include "matrix.h"


struct triangle {
	struct vec3 points[3];
};

struct mesh {
	struct vec3 *p;
	int *idx;
};


#endif
