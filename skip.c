/* usage: skip numbytes -- swallows that many bytes then passes on the rest */
/*        pass numbytes -- passes on that many bytes then stops listening */
/*        append numbytes -- adds that many zero bytes onto end */
/*        prepend numbytes -- adds that many zero bytes onto beginning */
/*        insert offset numbytes -- adds that many zero bytes at that place */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
extern char *__progname;
static void usage(void)
{
	if (strcmp(__progname, "insert") == 0)
		fprintf(stderr, "usage: %s offset bytes\n", __progname);
	else
		fprintf(stderr, "usage: %s bytes\n", __progname);
	exit(EXIT_FAILURE);
}
int main(int argc, char *argv[])
{
	int numbytes, offset = 0, c;

	if (strcmp(__progname, "insert") == 0)
	{
		if (argc != 3) usage();
		offset = atoi(argv[1]);
		numbytes = atoi(argv[2]);
	}
	else
	{
		if (argc != 2) usage();
		numbytes = atoi(argv[1]);
	}

	if (numbytes < 0) errx(1, "number of bytes must be nonnegative");
	if (offset < 0) errx(1, "offset must be nonnegative");

	if (strcmp(__progname, "append") == 0)
	{
		while ((c = getchar()) != EOF) putchar(c);
		while (numbytes-- > 0) putchar(0);
	}
	else if (strcmp(__progname, "prepend") == 0)
	{
		while (numbytes-- > 0) putchar(0);
		while ((c = getchar()) != EOF) putchar(c);
	}
	else if (strcmp(__progname, "insert") == 0)
	{
		while (offset-- > 0 && (c = getchar()) != EOF) putchar(c);
		while (numbytes-- > 0) putchar(0);
		while ((c = getchar()) != EOF) putchar(c);
	}
	else if (strcmp(__progname, "pass") == 0)
		while (numbytes-- > 0 && (c = getchar()) != EOF) putchar(c);
	else
	{
		while (numbytes-- > 0 && getchar() != EOF) ;
		while ((c = getchar()) != EOF) putchar(c);
	}
	return 0;
}
