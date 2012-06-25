#include <stdio.h>
#include <unistd.h>

#define BUFLEN 1024

int main(void)
{
	short oldbytes[BUFLEN], newbytes[BUFLEN];
	int ret;
	while ((ret = fread(oldbytes, sizeof oldbytes[0], BUFLEN, stdin)))
	{
		swab(oldbytes, newbytes, sizeof oldbytes[0] * BUFLEN);
		fwrite(newbytes, sizeof newbytes[0], BUFLEN, stdout);
	}
	return 0;
}
