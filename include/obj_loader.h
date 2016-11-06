#ifndef __OBJ_LOADER_H__
#define __OBJ_LOADER_H__

#include "vector.h"

typedef struct {
	int nverts;
	int nfaces;
	vector vertexes;
	vector faces;
} obj_loader;

void obj_loader_init(obj_loader *obj);

void obj_loader_finalize(obj_loader *obj);

void load_obj(obj_loader *obj, char *fname);

#endif
