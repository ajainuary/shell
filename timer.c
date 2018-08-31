#include <unistd.h>
#include <stdio.h>
int main()
{
	for (int i = 0; i < 5; ++i)
	{
		sleep(1);
		printf("%d Seconds elapsed\n", i);
	}
	return 0;
}