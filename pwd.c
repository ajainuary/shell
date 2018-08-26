#include <errno.h>
#include <string.h>
#include <unistd.h>
#define catch if (errno != 0){ \
    write(1, "Error: ", 7);    \
    write(1, strerror(errno), strlen(strerror(errno))); write(1, "\n", 1); }

int main(int argc, char const *argv[])
{
	if(argc != 1)
	{
		write(2, "Bad Option, No Command Line Arguments Expected\n", 47);
		return 1;
	}
	char path[4096];
	getcwd(path, 4096);
	catch;
	int length = strlen(path);
	path[length] = '\n';
	write(1, path, length+1);
	return 0;
}