#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "matrix.h"

int pe_cammove(struct mat4 *m, double x, double y, double z);

int pe_camrotate(struct mat4 *m, double angx, double angy, double angz);

#endif
