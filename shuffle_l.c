#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LINEMAXLEN 1088

int main(void)
{
	char **lines = NULL;
	int numlines = 0, sizelines = 0, i;

	srandom((unsigned int)time(NULL));

	for(;;)
	{
		if(++numlines>sizelines)
		{
			sizelines = sizelines*2+5;
			lines = realloc(lines, sizelines * sizeof(char *));
			if(lines==NULL)
			{
				fprintf(stderr, "out of memory\n");
				exit(1);
			}
		}

		lines[numlines-1] = malloc(LINEMAXLEN);
		if(lines[numlines-1]==NULL)
		{
			fprintf(stderr, "out of memory\n");
			exit(2);
		}
		if(!fgets(lines[numlines-1], LINEMAXLEN, stdin))
		{
			free(lines[--numlines]);
			break;
		}
	}

	for(i = numlines-1; i > 0; i--)
	{
		char *p;
		int r;
		p = lines[i];
		r = random() % (i+1);
		lines[i] = lines[r];
		lines[r] = p;
	}

	for(i = 0; i < numlines; i++)
		printf("%s", lines[i]);

	return 0;
}
