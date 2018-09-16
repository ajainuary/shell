#ifndef ABCD
#define ABCD ABCD
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
#include <signal.h>

#define catch if (errno != 0){printf("Error: %s\n", strerror(errno)); errno = 0; return;}

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
void free_args();
void prompt();
void read_cmd();
void interpret(int n);
void pwd();
void cd();
void echo();
void execute_cmd();
void pinfo();
void perms(int perm[][3], struct stat *test);
void ls();
void remindme();
void clock_rtc();
void clock_wrapper();
void get_user(int uid, char *uname);
void sig_child_process_terminated(int sig);
extern int _is_background;
#endif