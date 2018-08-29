#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define catch if (errno != 0){ \
    write(1, "Error: ", 7);    \
    write(1, strerror(errno), strlen(strerror(errno))); write(1, "\n", 1); }

int main(int argc, char *argv[], char *envp[]) {
  char **arg = malloc(sizeof(*arg)*2);
  *arg = "ls";
  *(arg+1) = "-l";
  execvp("ls", arg);
  catch;
}