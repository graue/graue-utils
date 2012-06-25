/*
this can be invoked as "loop" or "reverse"
loop: consume all input, then output it over and over again indefinitely
reverse x: consume all input, then output it once in reverse order
           in x-byte chunks (x assumed to be 1 if not given). incomplete
           chunks at the end are ignored. (e.g. "reverse 6" on 34 byte
           input -> 30 byte output)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readfile.h"

extern char *__progname;

int main(int argc, char *argv[])
{
	char *buf;
	int len;
	int ix, jx;
	int chunksize = 1;

	buf = readfile(stdin, &len);

	if (strcmp(__progname, "reverse") == 0)
	{
		if (argc >= 2 && atoi(argv[1]) > 0)
			chunksize = atoi(argv[1]);
		ix = len;
		ix -= (len % chunksize) - chunksize;
		for (; ix >= 0; ix -= chunksize)
			for (jx = 0; jx < chunksize; jx++)
				putchar(buf[ix+jx]);
	}
	else for (;;) /* loop */
	{
		for (ix = 0; ix < len; ix++)
			putchar(buf[ix]);
	}

	return 0;
}
