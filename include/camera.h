#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"

int pe_cammove(struct mat4 *m, double x, double y, double z);

int pe_camrotate(struct mat4 *m, double angx, double angy, double angz);

#endif
