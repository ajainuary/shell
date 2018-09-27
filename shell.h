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
#include <linux/unistd.h>
#define catch if (errno != 0){printf("Error: %s\n", strerror(errno)); errno = 0; return;}

#define MAX_READ 1000000
#define WHITESPACE " \t\r\a\n"
struct Info {
  int uid;
  char uname[4096];
};
extern char *commands[256];
extern char home[4096];
extern char *arg[32767];
extern char argcount;
extern void free_args();
extern void prompt();
extern void read_cmd();
extern void interpret(char *cmd);
extern void pwd();
extern void cd();
extern void echo();
extern void execute_cmd();
extern void pinfo();
extern void perms(int perm[][3], struct stat *test);
extern void ls();
extern void remindme();
extern void clock_rtc();
extern void clock_wrapper();
extern void get_user(int uid, char *uname);
extern void sig_child_process_terminated(int sig);
extern void Mysetenv();
extern void Myunsetenv();
extern void jobs();
extern int _is_background;
#endif