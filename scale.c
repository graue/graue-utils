// from a file posted Sep 11, 2006

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "strl.h" // strlcpy, strlcat for linux

#define ACOUNT(a) ((int)(sizeof (a) / sizeof (a)[0]))

static const char *notessharp[] =
{
	"C", "C#", "D", "D#", "E", "F",
	"F#", "G", "G#", "A", "A#", "B"
};

static const char *notesflat[] =
{
	"C", "Db", "D", "Eb", "E", "F",
	"Gb", "G", "Ab", "A", "Bb", "B"
};

#define NUMNOTES (ACOUNT(notessharp))

#define MAXINTERVALS 8

typedef struct
{
	const char *name;
	const int intervals[MAXINTERVALS];
} scale_t;

static const scale_t scales[] =
{
	{"major",		{2, 2, 1, 2, 2, 2, 1}}, /* same as ionian */
	{"ionian",		{2, 2, 1, 2, 2, 2, 1}},
	{"dorian",		{2, 1, 2, 2, 2, 1, 2}},
	{"phrygian",		{1, 2, 2, 2, 1, 2, 2}},
	{"lydian",		{2, 2, 2, 1, 2, 2, 1}},
	{"mixolydian",		{2, 2, 1, 2, 2, 1, 2}},
	{"aeolian",		{2, 1, 2, 2, 1, 2, 2}},
	{"locrian",		{1, 2, 2, 1, 2, 2, 2}},

	{"lydian dominant",	{2, 2, 2, 1, 2, 1, 2}},
	{"altered lydian",	{2, 2, 2, 2, 1, 2, 1}},
	{"altered",		{1, 2, 1, 2, 2, 2, 2}},

	{"harmonic major",	{2, 2, 1, 2, 1, 3, 1}},
	{"harmonic minor",	{2, 1, 2, 2, 1, 3, 1}},

	{"augmented",		{2, 2, 1, 3, 1, 2, 1}},
	{"diminished",		{2, 1, 2, 1, 3, 2, 1}},
	{"octatonic",		{2, 1, 2, 1, 2, 1, 2, 1}},
	{"whole tone",		{2, 2, 2, 2, 2, 2}},

	{"major bebop",		{2, 2, 1, 2, 1, 1, 2, 1}},
	{"minor bebop",		{2, 1, 2, 2, 1, 1, 2, 1}},
	{"major blues",		{2, 1, 1, 3, 1, 1, 3}},
	{"minor blues",		{3, 2, 1, 1, 3, 1, 1}},
	{"major pentatonic",	{2, 2, 3, 2, 3}},
	{"minor pentatonic",	{3, 2, 2, 3, 2}},

	{"arabian",		{2, 2, 2, 1, 2, 2, 2}},
	{"arabic",		{1, 3, 1, 2, 1, 3, 1}}, /* double harmonic
							   / gypsy */
	{"balinese",		{1, 2, 4, 1, 4}},
	{"enigmatic",		{1, 3, 2, 2, 2, 1, 1}},
	{"hindu",		{2, 2, 1, 2, 1, 2, 2}},
	{"hirajoshi",		{2, 1, 4, 1, 4}},
	{"hungarian gypsy",	{2, 1, 3, 1, 1, 2, 2}},
	{"hungarian major",	{2, 1, 3, 1, 1, 3, 1}},
	{"hungarian minor",	{3, 1, 2, 1, 2, 1, 2}},
	{"iwato",		{1, 4, 1, 4, 2}},
	{"kumoi",		{1, 4, 2, 1, 4}},
	{"leading whole tone",	{2, 2, 2, 2, 2, 1, 1}},
	{"lydian minor",	{2, 2, 2, 1, 1, 2, 2}},
	{"major locrian",	{2, 2, 1, 1, 2, 2, 2}},
	{"neapolitan major",	{1, 2, 2, 2, 2, 2, 1}},
	{"neapolitan minor",	{1, 2, 2, 2, 1, 3, 1}},
	{"oriental",		{1, 3, 1, 1, 3, 1, 2}},
	{"pelog",		{1, 2, 4, 3, 2}},
	{"spanish 8 tone",	{1, 2, 1, 1, 1, 2, 2, 2}},
	{"spanish gypsy",	{1, 3, 1, 2, 1, 2, 2}}, /* spanish phrygian */

	{"not augmented",	{3, 1, 3, 1, 3, 1}}, /* I don't know */
	{"not melodic minor",	{2, 1, 2, 2, 2, 2, 1}}, /* I don't know */
};

extern char *__progname;

/*
static void usage(void)
{
	fprintf(stderr, "usage: %s [note] [scale-name]\n", __progname);
	exit(EXIT_FAILURE);
}
*/

static void printscale(int startnote, int scale, int useflats)
{
	int ix;
	int note = startnote;
	const char **notes = notessharp;

	if (useflats)
		notes = notesflat;

	printf("%s", notes[note]);
	for (ix = 0; ix < MAXINTERVALS && scales[scale].intervals[ix] > 0; ix++)
	{
		note = (note + scales[scale].intervals[ix]) % NUMNOTES;
		printf(" %s", notes[note]);
	}
	putchar('\n');
}

int main(int argc, char *argv[])
{
	char scalename[4000];
	int scale, note;
	int ix;
	int useflats = 0;

	argc--, argv++;

	if (argc == 0)
	{
		for (scale = 0; scale < ACOUNT(scales); scale++)
			printf("%s\n", scales[scale].name);
		return 0;
	}

	/* '-f' option: use flats instead of sharps */
	if (strcmp(argv[0], "-f") == 0 && argc >= 2)
	{
		useflats = 1;
		argc--, argv++;
	}

	for (note = 0; note < NUMNOTES; note++)
	{
		if (strcasecmp(notessharp[note], argv[0]) == 0)
			break;
		if (strcasecmp(notesflat[note], argv[0]) == 0)
		{
			useflats = 1;
			break;
		}
	}

	if (note == NUMNOTES)
		errx(1, "%s is not a note", argv[0]);

	argc--, argv++;

	if (argc == 0)
	{
		for (scale = 0; scale < ACOUNT(scales); scale++)
		{
			printf("%s: ", scales[scale].name);
			printscale(note, scale, useflats);
		}
		return 0;
	}

	strlcpy(scalename, argv[0], sizeof scalename);
	for (ix = 1; ix < argc; ix++)
	{
		strlcat(scalename, " ", sizeof scalename);
		strlcat(scalename, argv[ix], sizeof scalename);
	}

	for (scale = 0; scale < ACOUNT(scales); scale++)
		if (strcasecmp(scales[scale].name, scalename) == 0)
			break;

	if (scale == ACOUNT(scales))
		errx(1, "%s: don't know that scale", scalename);

	printscale(note, scale, useflats);
	return 0;
}
