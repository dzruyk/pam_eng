#ifndef __OBJ_LOADER_H__
#define __OBJ_LOADER_H__

#include "dbuf.h"

typedef struct {
	int nverts;
	int nfaces;
	dbuf vertexes;
	dbuf faces;
} obj_loader;

void obj_loader_init(obj_loader *obj);

void obj_loader_finalize(obj_loader *obj);

void load_obj(obj_loader *obj, const char *fname);

#endif

