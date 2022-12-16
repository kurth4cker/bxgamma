/*
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

#include <stdio.h>
#include <errno.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/xf86vmode.h>
#include <ctype.h>
#include <stdlib.h>

static char *program_name;
static int major_version, minor_version;
static int event_base, error_base;

/* Minimum extension version required */
#define MINMAJOR 2
#define MINMINOR 0

/* Maximum and Minimum gamma values */
#define GAMMA_MIN 0.1f
#define GAMMA_MAX 10.0f

static void
syntax(const char *errmsg)
{
	if (errmsg != NULL)
		fprintf (stderr, "%s: %s\n\n", program_name, errmsg);

	fprintf (stderr, "usage:  %s [-options]\n\n%s", program_name,
"where the available options are:\n"
"    -display host:dpy       or -d\n"
"    -quiet                  or -q\n"
"    -screen                 or -s\n"
"    -version                or -v\n"
"    -gamma f.f              Gamma Value\n"
"    -rgamma f.f             Red Gamma Value\n"
"    -ggamma f.f             Green Gamma Value\n"
"    -bgamma f.f             Blue Gamma Value\n\n"
"If no gamma is specified, returns the current setting\n");
	exit (1);
}


/*
 * The following is a hack until XrmParseCommand is ready.  It determines
 * whether or not the given string is an abbreviation of the arg.
 */

static int 
isabbreviation(const char *arg, const char *s, size_t minslen)
{
	size_t arglen;
	size_t slen;

	/* exact match */
	if (strcmp (arg, s) == 0) return (True);

	arglen = strlen (arg);
	slen = strlen (s);

	/* too long or too short */
	if (slen >= arglen || slen < minslen) return (False);

	/* abbreviation */
	if (strncmp (arg, s, slen) == 0) return (True);

	/* bad */
	return (False);
}

int
main(int argc, char **argv)
{
	int ret = 2;
	char *displayname = NULL;
	Display *dpy;
	float bgam = -1.0f;
	XF86VidModeGamma gamma;
	int quiet = 0;
	int screen = -1;
	int ch;

	program_name = argv[0];

	while ((ch = getopt(argc, argv, "d:s:qv")) != -1)
		switch (ch) {
		case 'd':
			if (!optarg)
				syntax("-d requires an argument");
			displayname = optarg;
			break;
		case 's':
			if (!optarg)
				syntax("-s requires an argument");
			screen = atoi(optarg);
			break;
		case 'q':
			quiet = 1;
			break;
		case 'v':
			puts(PACKAGE_STRING);
			return 0;
		case 'h':
			syntax(NULL);
		}

	if (optind < argc)
		bgam = strtof(argv[optind], NULL);

	if ((dpy = XOpenDisplay(displayname)) == NULL) {
		fprintf (stderr, "%s:  unable to open display '%s'\n",
				program_name, XDisplayName (displayname));
		exit(1);
	} else if (screen == -1)
		screen = DefaultScreen(dpy);

	if (!XF86VidModeQueryVersion(dpy, &major_version, &minor_version)) {
		fprintf(stderr, "Unable to query video extension version\n");
		goto finish;
	}

	if (!XF86VidModeQueryExtension(dpy, &event_base, &error_base)) {
		fprintf(stderr, "Unable to query video extension information\n");
		goto finish;
	}

	/* Fail if the extension version in the server is too old */
	if (major_version < MINMAJOR ||
			(major_version == MINMAJOR && minor_version < MINMINOR)) {
		fprintf(stderr,
				"Xserver is running an old XFree86-VidModeExtension version"
				" (%d.%d)\n", major_version, minor_version);
		fprintf(stderr, "Minimum required version is %d.%d\n",
				MINMAJOR, MINMINOR);
		goto finish;
	}

	if (!XF86VidModeGetGamma(dpy, screen, &gamma)) {
		fprintf(stderr, "Unable to query gamma correction\n");
		goto finish;
	} else if (!quiet)
		printf("blue: %6.3f\n", (double)gamma.blue);

	if (bgam >= 0.0f)
		gamma.blue = bgam;
	else {
		/* Not changing gamma, all done */
		ret = 0;
		goto finish;
	}

	/* Change gamma now */
	if (!XF86VidModeSetGamma(dpy, screen, &gamma))
		fprintf(stderr, "Unable to set gamma correction\n");
	else if (!XF86VidModeGetGamma(dpy, screen, &gamma))
		fprintf(stderr, "Unable to query gamma correction\n");
	else {
		ret = 0; /* Success! */
		if (!quiet) {
			printf("blue: %6.3f\n",	(double)gamma.blue);
		}
	}

finish:
	XCloseDisplay (dpy);
	return ret;
