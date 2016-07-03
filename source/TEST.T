
#include <stdio.h>
#include <assert.h>
#include <bios.h>
#include <conio.h>
#include <ctype.h>
#include <direct.h>
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <getopt.h>
#include <graph.h>
#include <io.h>
#include <lex.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <process.h>
#include <search.h>
#include <setjmp.h>
#include <share.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/locking.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/utime.h>
#include <time.h>
#include <varargs.h>


#ifdef SUN
#include <sys/time.h>
#endif

#include "shroud.h"

#pragma pack ()
#pragma pack (2)

#line 69 "test.c"

main (int argcount, char *argvalue [])
{
	int i;
	unsigned int u;
	long l;
	double d;

	/* test float constants */
	d = 4.4e-44f;
	d = 4.4e-44l;
	d = 4.4e+44f;
	d = 4.4e+44l;
	d = 4.4e44f;
	d = 4.4e44l;

	d = .1e11;
	d = 0.1e11;
	d = 0.e11;

	d = .1e+11;
	d = 0.1e+11;
	d = 0.e+11;

	d = .1e-11;
	d = 0.1e-11;
	d = 0.e-11;

	/* test integer constants */
	u = 4u;
	u = 4U;
	u = 0xf4u;
	u = 0xe4U;

	l = 4l;
	l = 4L;
	l = 0xd4l;
	l = 0xc4L;

	for (i = 0; i < argcount; i++)
	{
		printf ("%s ", argvalue [i]);
	}
	printf ("\n");
}

