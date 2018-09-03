#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <syscall.h>
#include <time.h>
#include <unistd.h>

#define catch if (errno != 0){write(1, "Error: ", 7); write( \
    1, strerror(errno), strlen(strerror(errno)));            \
                              write(1, "\n", 1); errno = 0; return;}

#define print(x) write(1, x, strlen(x))
#define MAX_READ 1000000
#define WHITESPACE " \t\r\a\n"
struct Info {
  int uid;
  char uname[4096];
};
char *commands[256];
char home[4096];
char *arg[32767];
char argcount;