/*
 * sponge: Sucks up all its input and only then writes it
 * to the given file. Example:
 *
 *   sort <foo | sponge foo       -- sorts foo in-place
 *
 * This works by creating a temporary file to save all the
 * input in. Later we move this to the given filename,
 * overwriting that file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h> /* mode_t */
#include <sys/stat.h> /* umask() */
#include <sys/param.h> /* MAXPATHLEN */
#include "strl.h" /* strlcpy, strlcat for Linux */

extern char *__progname;

static void usage(void)
{
	fprintf(stderr, "usage: %s outputfile\n", __progname);
	exit(EXIT_FAILURE);
}

static char tmppath[MAXPATHLEN+1];
static char outfile[MAXPATHLEN+1];

int main(int argc, char *argv[])
{
	int fd;
	FILE *fp;
	char *p;
	int c;
	mode_t myumask;
	mode_t filemode;

	if (argc != 2)
		usage();
	if (strlcpy(outfile, argv[1], sizeof outfile) >= sizeof outfile)
		errx(1, "filename too long: %s", argv[1]);
	strlcpy(tmppath, outfile, sizeof tmppath);

	/* Use a temp file in the same directory (thus the same partition). */
	if ((p = strrchr(tmppath, '/')) != NULL)
		*p = '\0';
	if (strlcat(tmppath, ".XXXXXXXXXX", sizeof tmppath) >= sizeof tmppath)
		errx(1, "filename too long: %s", outfile);

	if ((fd = mkstemp(tmppath)) == -1)
		err(1, "cannot get temporary file");
	if ((fp = fdopen(fd, "wb")) == NULL)
	{
		unlink(tmppath);
		close(fd);
		err(1, "cannot open temporary file after creating it");
	}

	while ((c = getchar()) != EOF)
		putc(c, fp);
	fclose(fp);

	/* Now link the new file over the old if it exists. */
	if (unlink(outfile) == -1 && errno != ENOENT)
		err(1, "cannot unlink %s", outfile);
	if (link(tmppath, outfile) == -1)
		err(1, "cannot link %s to %s", outfile, tmppath);
	if (unlink(tmppath) == -1)
		warn("cannot unlink temp file %s", tmppath);

	/* Get current umask. */
	myumask = umask(0);
	(void)umask(myumask); /* Set the old one again. */

	filemode = (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
		& ~myumask;
	if (chmod(outfile, filemode) == -1)
		warn("could not change mode of %s", outfile);

	return 0;
}
