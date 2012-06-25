#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char cmd[200];

	srandomdev();
	snprintf(cmd, 200, "shuffle | head -n %d",
		(int) (9 + random()%10));
	system(cmd);
	return 0;
}
