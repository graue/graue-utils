/* converts between voltage (not power!) ratio and dB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern char *__progname;
static void usage(void)
{
	fprintf(stderr, "usage: %s val\n", __progname);
	exit(EXIT_FAILURE);
}

/*
 * To convert between *power* ratios, use 10.0f instead of 20.0f
 * in each of the below functions.
 */

double rattodb(double f)
{
	return 20.0f*log10(f);
}

double dbtorat(double f)
{
	return pow(10.0f, f / 20.0f);
}

int main(int argc, char *argv[])
{
	if (argc != 2) usage();
	if (strcmp(__progname, "rattodb") == 0)
		printf("%f\n", rattodb(atof(argv[1])));
	else
		printf("%f\n", dbtorat(atof(argv[1])));
	return 0;
}
