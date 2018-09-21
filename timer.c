#include <stdio.h>
#include <unistd.h>

int main()
{
	sleep(1);
	char s[4096];
	scanf("%s", s);
	printf("%s\n", s);
	fflush(stdout);
	return 0;
}