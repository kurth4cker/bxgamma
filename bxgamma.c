/*
 * Copyright 2022,2023 kurth4cker
 * Copyright 1999  The XFree86 Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL 
 * THE XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF 
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 * 
 * Written by David Bateman
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

/* Maximum and Minimum gamma values */
#define GAMMA_MIN 1
#define GAMMA_MAX 100

static Display *dpy;

void eprintf(const char *fmt, ...)
{
	va_list vlist;
	va_start(vlist, fmt);
	vfprintf(stderr, fmt, vlist);
	va_end(vlist);

	XCloseDisplay(dpy);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	XF86VidModeGamma gamma;
	int bgam = -1;
	int screen;
	int ch;

	while ((ch = getopt(argc, argv, "hv")) != -1)
		switch (ch) {
		case 'v':
			puts(PACKAGE_STRING);
			return 0;
			break;
		case 'h':
			puts("see bxgamma(1)");
			return 0;
			break;
		case '?':
			return 1;
		}

	if (optind < argc) {
		bgam = atoi(argv[optind]);
		if (bgam < GAMMA_MIN || bgam > GAMMA_MAX)
			eprintf( "gamma values must be between %d and %d\n",
					GAMMA_MIN, GAMMA_MAX);
	}

	if (!(dpy = XOpenDisplay(NULL)))
		eprintf("unable to open display '%s'\n", XDisplayName(NULL));

	screen = DefaultScreen(dpy);

	if (!XF86VidModeGetGamma(dpy, screen, &gamma))
		eprintf("unable to query gamma correction\n");

	if (bgam >= 0) {
		gamma.blue = bgam / 10.0f;
		if (!XF86VidModeSetGamma(dpy, screen, &gamma))
			eprintf("unable to set gamma correction");
	}
	else {
		printf("%d\n", (int)(10 * gamma.blue));
	}

	XCloseDisplay(dpy);
}
