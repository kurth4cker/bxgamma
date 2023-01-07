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

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

static int major_version, minor_version;
static int event_base, error_base;

/* Minimum extension version required */
#define MINMAJOR 2
#define MINMINOR 0

/* Maximum and Minimum gamma values */
#define GAMMA_MIN 0.1f
#define GAMMA_MAX 10.0f

int main(int argc, char **argv)
{
	int ret = 2;
	const char *displayname = NULL;
	Display *dpy;
	float bgam = -1.0f;
	XF86VidModeGamma gamma;
	int quiet = 0;
	int screen = -1;
	int ch;

	while ((ch = getopt(argc, argv, "d:s:qhv")) != -1)
		switch (ch) {
		case 'd':
			if (!optarg)
				return 1;
			displayname = optarg;
			break;
		case 's':
			if (!optarg)
				return 1;
			screen = atoi(optarg);
			break;
		case 'q':
			quiet = 1;
			break;
		case 'v':
			puts(PACKAGE_STRING);
			break;
		case 'h':
			puts("see bxgamma(1)");
			break;
		}

	if (optind < argc) {
		bgam = strtof(argv[optind], NULL);
		if (bgam < GAMMA_MIN || bgam > GAMMA_MAX) {
			fprintf(stderr, "gamma values must be between %6.3f and %6.3f\n",
					(double)GAMMA_MIN, (double)GAMMA_MAX);
			return 1;
		}
	}

	if ((dpy = XOpenDisplay(displayname)) == NULL) {
		fprintf(stderr, "unable to open display '%s'\n", XDisplayName(displayname));
		return 1;
	}
	else if (screen == -1)
		screen = DefaultScreen(dpy);

	if (!XF86VidModeQueryVersion(dpy, &major_version, &minor_version)) {
		fputs("unable to query video extension version\n", stderr);
		goto finish;
	}

	if (!XF86VidModeQueryExtension(dpy, &event_base, &error_base)) {
		fputs("unable to query video extension information\n", stderr);
		goto finish;
	}

	/* Fail if the extension version in the server is too old */
	if (major_version < MINMAJOR ||
			(major_version == MINMAJOR && minor_version < MINMINOR)) {
		fprintf(stderr,
				"Xserver is running an old XFree86-VidModeExtension version"
				" (%d.%d)\n", major_version, minor_version);
		fprintf(stderr, "minimum required version is %d.%d\n",
				MINMAJOR, MINMINOR);
		goto finish;
	}

	if (!XF86VidModeGetGamma(dpy, screen, &gamma)) {
		fputs("unable to query gamma correction\n", stderr);
		goto finish;
	}
	else if (!quiet)
		printf("blue: %.3f\n", (double)gamma.blue);

	if (bgam >= 0.0f)
		gamma.blue = bgam;
	else {
		/* Not changing gamma, all done */
		ret = 0;
		goto finish;
	}

	/* Change gamma now */
	if (!XF86VidModeSetGamma(dpy, screen, &gamma))
		fputs("unable to set gamma correction\n", stderr);
	else
		ret = 0; /* Success! */

finish:
	XCloseDisplay(dpy);
	return ret;
}
