#ifndef __GUI_H__
#define __GUI_H__

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>

struct xdata {
	xcb_connection_t *connection;
	xcb_screen_t *screen;
	xcb_window_t win;

	cairo_t *cr;
	cairo_surface_t *xcbsur;
	cairo_surface_t *sur;

	int (*defaultcallback)(void *userdata);
	int (*drawcallback)(cairo_surface_t *sur, void *userdata);
	int (*keypresscallback)(int keycode, void *userdata);
};

int initgui(struct xdata *guidata, int w, int h);

int windowforceredraw(xcb_connection_t *c, xcb_window_t *win);

int mainloop(struct xdata *guidata, void *userdata);

int settimer(void (*timerfunc)(int), void *userdata, double secs);

#endif
