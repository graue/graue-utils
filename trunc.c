/* Program to truncate a file. Public domain. */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char *filename = NULL;
	off_t newlen;

	if (argc != 3)
	{
		fprintf(stderr, "usage: trunc file newlen\n");
		exit(1);
	}

	newlen = (off_t) strtol(argv[2], NULL, 10);
	filename = argv[1];

	if (truncate(filename, newlen))
	{
		perror(filename);
		exit(1);
	}

	return 0;
}
