#include <stdio.h>
#include "xm.h"

void *readfile(FILE *fp, int *len)
{
	char *buf = NULL;
	int nbuf = 0, sbuf = 0;
	int c;

	while ((c = getc(fp)) != EOF)
	{
		XPND(buf, nbuf, sbuf);
		buf[nbuf++] = c;
	}
	*len = nbuf;
	return buf;
}
