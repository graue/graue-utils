#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <inttypes.h>

int main(void)
{
	uintmax_t count = 0;
	char buf[50000];

	for (;;)
	{
		uintmax_t num;

		if (NULL == gets(buf)) // XXX - yes, insecure, lazy etc.
			break;
		num = strtoumax(buf, NULL, 0);
		count += num;
	}

	printf("%llu\n", (unsigned long long)count);
	return 0;
}
