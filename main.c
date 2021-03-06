#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <libgen.h>

#include "objloader.h"
#include "macros.h"
#include "mesh.h"
#include "matrix.h"
#include "render.h"
#include "timer.h"
#include "camera.h"
#include "gui.h"

#define SURWIDTH 1280
#define SURHEIGHT 720

char *usg = "usage: %s [OPTS] input.obj\n";

int drag = 0;

struct renderdata {
	struct pe_context context;
	struct mesh m;
	struct pe_material mat;
	struct pe_surface sur;
};

void
usage(const char *progpath)
{
	char *p, *pname;

	p = strdup(progpath);

	if (p == NULL)
		pname = (char *) progpath;
	else
		pname = basename(p);

	printf(usg, pname);

	exit(1);
}

int
render(void *userdata)
{
	struct renderdata *rd;
	int start, stop;

	start = pe_getticks();

	rd = userdata;

	pe_fillsur(rd->context.target, 0.0, 0.0, 0.0);

	pe_setvertex(&(rd->context),
		(const struct dbuf *) &(rd->m.vertex));
	pe_setindex(&(rd->context),
		(const struct dbuf *) &(rd->m.idx));
	pe_setnorm(&(rd->context),
		(const struct dbuf *) &(rd->m.norm));
	pe_settexcoord(&(rd->context),
		(const struct dbuf *) &(rd->m.text));
	pe_setmaterial(&(rd->context), &(rd->mat));

	pe_render(&(rd->context));
	stop = pe_getticks();

	printf("render time %d\n", stop - start);

	return 0;
}

int
draw(cairo_surface_t *sur, void *userdata)
{
	struct renderdata *rd;
	int surw, surh;
	unsigned char *destline, *srcline;
	int deststride;
	int i, j;

	rd = userdata;

	destline = cairo_image_surface_get_data(sur);
	deststride = cairo_image_surface_get_stride(sur);
	surw = cairo_image_surface_get_width(sur);
	surh = cairo_image_surface_get_height(sur);

	srcline = rd->sur.data + (surh - 1) * surw * 3;

	for (i = 0; i < surh; ++i) {
		for (j = 0; j < surw; ++j) {
			destline[j * 4 + 0] = srcline[j * 3 + 2];
			destline[j * 4 + 1] = srcline[j * 3 + 1];
			destline[j * 4 + 2] = srcline[j * 3 + 0];
		}

		destline += deststride;
		srcline -= surw * 3;
	}

	return 0;
}

int
keypress(xcb_keysym_t keysym, void *userdata)
{
	struct renderdata *rd;
	double movedelta;

	rd = userdata;

	movedelta = 0.10;

	printf("Keysym: %x\n", keysym);

	switch(keysym) {
	case XKB_KEY_w:
		pe_cammove(&(rd->context.worldmat), 0.0, 0.0, movedelta);
		break;

	case XKB_KEY_s:
		pe_cammove(&(rd->context.worldmat), 0.0, 0.0, -movedelta);
		break;

	case XKB_KEY_a:
		pe_cammove(&(rd->context.worldmat), -movedelta, 0.0, 0.0);
		break;

	case XKB_KEY_d:
		pe_cammove(&(rd->context.worldmat), movedelta, 0.0, 0.0);
		break;

	case XKB_KEY_q:
		pe_cammove(&(rd->context.worldmat), 0.0, -movedelta, 0.0);
		break;

	case XKB_KEY_e:
		pe_cammove(&(rd->context.worldmat), 0.0, movedelta, 0.0);
		break;

	default:
		break;
	}

	return 0;
}

int
motion(int x, int y, void *userdata)
{
	static int prevx = -1, prevy = -1;

	struct renderdata *rd;
	double dx, dy;

	rd = userdata;

	if (prevx == -1 || prevy == -1) {
		prevx = x;
		prevy = y;
	}

	if (drag) {
		dx = ((double) prevx - x) / rd->sur.w;
		dy = ((double) prevy - y) / rd->sur.h;

		pe_camrotate(&(rd->context.worldmat), dy, dx, 0.0);
	}

	prevx = x;
	prevy = y;

	return 0;
}

int
buttonpress(xcb_button_t buttoncode, void *userdata)
{
	drag = (buttoncode == 1) ? 1 : drag;

	return 0;
}

int
buttonrelease(xcb_button_t buttoncode, void *userdata)
{
	drag = (buttoncode == 1) ? 0 : drag;

	return 0;
}

int
main(int argc, char **argv)
{
	struct renderdata rd;
	struct xdata guidata;
	struct mat4 perspmat;

	if (argc < 2)
		usage(argv[0]);

	initgui(&guidata, SURWIDTH, SURHEIGHT);

	pe_meshinit(&(rd.m));

	pe_objload(&(rd.m), argv[1]);

	//pe_meshnormalize(&(rd.m));

	if (pe_createsur(&(rd.sur), SURWIDTH, SURHEIGHT, SF_RGB24) < 0)
		return 1;

	rd.mat.color.r = 0.0;
	rd.mat.color.g = 1.0;
	rd.mat.color.b = 0.0;
	rd.mat.color.a = 1.0;

	pe_initcontext(&(rd.context));
	pe_settarget(&(rd.context), &(rd.sur));

	pe_setperspmatrix(&(rd.context),
		mat4persp(&perspmat, 1, 100, -1, 1, -1, 1));

//	rd.context.conf.wired = 1;

	pe_cammove(&rd.context.worldmat, 0, 0, -2.);

	guidata.defaultcallback = render;
	guidata.drawcallback = draw;
	guidata.keypresscallback = keypress;
	guidata.motioncallback = motion;
	guidata.buttonpresscallback = buttonpress;
	guidata.buttonreleasecallback = buttonrelease;

	mainloop(&guidata, &rd);

	pe_meshclean(&(rd.m));

	return 0;
}
