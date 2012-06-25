/*

A cat-like program that buffers input data ahead
of time even if it's not immediately needed.

*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <poll.h>

static void usage(void)
{
	fprintf(stderr, "usage: capture [buffer size]\n");
	exit(EXIT_FAILURE);
}

static int bufsize = 50000;
static char *buf;
static int verbose = 0;

static void capture(void)
{
	struct pollfd fds[2];
	ssize_t written, bytesread;
	int ret;
	int buffill = 0; // how much of buffer is filled
	int ateof = 0; // eof or stdin closed

	fds[0].fd = STDIN_FILENO;
	fds[1].fd = STDOUT_FILENO;

	// set stdout to non-blocking so we can try writing _up to_
	// however much we have in the buffer, while getting an immediate
	// return
	if (fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK) == -1)
		err(1, "can't set stdout to non-blocking");

	// also stdin (Tgk)
	if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == -1)
		err(1, "can't set stdout to non-blocking");

	for (;;)
	{
		// do we want to read? if buffer is not full and we aren't
		// at eof yet, then yes we do
		if (buffill < bufsize && !ateof)
			fds[0].events = POLLIN;
		else
			fds[0].events = 0;

		// do we want to write? if buffer is not empty, then yes we do
		if (buffill > 0)
			fds[1].events = POLLOUT;
		else
			fds[1].events = 0;

		ret = poll(fds, 2, INFTIM);
		if (ret == 0)
			errx(1, "bad return value from poll");
		if (ret == -1)
			err(1, "poll");
		if (fds[1].revents & (POLLERR|POLLNVAL))
			errx(1, "poll error/invalid");
		if (fds[1].revents & POLLHUP) // output closed so we're done
			exit(EXIT_SUCCESS);
		if (fds[0].revents & (POLLERR|POLLNVAL))
			errx(1, "poll error/invalid");
		if (fds[0].revents & POLLHUP) // input closed
			ateof = 1;

		if (buffill > 0 && fds[1].revents & POLLOUT)
		{
			// We can copy input to output, so do this.
			written = write(STDOUT_FILENO, buf, buffill);
			if (written == -1 && errno == EAGAIN)
			{
				warn("couldn't write despite POLLOUT set");
				written = 0;
			}
			else if (written == -1)
				err(1, "write");
			if (written > buffill)
				errx(1, "wrote too much");
			buffill -= written;

			if (buffill > 0)
				memmove(buf, &buf[written], buffill);
		}
		if (buffill < bufsize && fds[0].revents & POLLIN)
		{
			// We can read input, so do this.
			bytesread = read(STDIN_FILENO, &buf[buffill],
				bufsize - buffill);

			// Handle input error or end of file.
			if (bytesread == -1)
				exit(EXIT_FAILURE);
			if (bytesread == 0)
				ateof = 1;

			buffill += bytesread;
		}

		// all input written out?
		if (ateof && buffill == 0)
			exit(EXIT_SUCCESS);

		if (verbose)
			fprintf(stderr, "\rbuffer: %10d/%d", buffill, bufsize);
	}
}

int main(int argc, char *argv[])
{
	int myargc = argc;
	char **myargv = argv;

	myargc--, myargv++;
	if (myargc && myargv[0][0] == '-')
	{
		if (!strcmp(myargv[0], "--verbose"))
			verbose = 1;
		else
			usage();
		myargc--, myargv++;
	}
	if (myargc)
	{
		bufsize = atoi(myargv[0]);
		if (bufsize < 1)
			errx(1, "buffer size is too small: %d", bufsize);
		myargc--, myargv++;
	}
	if (myargc)
		usage();

	buf = malloc(bufsize);
	if (buf == NULL)
		err(1, "out of memory allocating buffer");

	capture();

	// NOTREACHED
	return 0;
}
