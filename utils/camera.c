#include "camera.h"

int pe_cammove(struct mat4 *m, double dx, double dy, double dz)
{
	m->_14 += dx;
	m->_24 += dy;
	m->_34 += dz;

	return 0;
}

int pe_camrotate(struct mat4 *m, double angx, double angy, double angz)
{
	struct mat4 rotm;

	if (angx != 0) {
		mat4rotate(&rotm, angx, DIM_X);
		mat4mult(m, m, &rotm);
	}

	if (angy != 0) {
		mat4rotate(&rotm, angy, DIM_Y);
		mat4mult(m, m, &rotm);
	}

	if (angz != 0) {
		mat4rotate(&rotm, angz, DIM_Z);
		mat4mult(m, m, &rotm);
	}

	return 0;
}
