#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>
#define catch if (errno != 0){ \
    write(1, "Error: ", 7);    \
    write(1, strerror(errno), strlen(strerror(errno))); write(1, "\n", 1); }
#define print(x) write(1, x, strlen(x))
#define MAX_READ 1000000

struct Info {
  int uid;
  char uname[4096];
  char home[4096];
};

void details(struct Info *instance) {
  instance->uid = getuid();
  int list = open("/etc/passwd", O_RDONLY);
  catch;
  char buffer[MAX_READ + 1];
  ssize_t sz = read(list, buffer, MAX_READ);
  catch;
  for (char *s = strtok(buffer, "\n"); s != NULL; s = strtok(NULL, "\n")) {
    char *name = strtok(NULL, ":");
    strtok(NULL, ":");  // Cycle through the password
    char *uid = strtok(NULL, ":");
    if (uid != NULL && atoi(uid) == instance->uid) {
      strtok(NULL, ":");  // Cycle through Group ID
      strtok(NULL, ":");  // Cycle through extra info
      char *home = strtok(NULL, ":");
      strcpy(instance->uname, name);
      strcpy(instance->home, home);
      break;
    }
  }
  return;
}

void prompt() {
  char path[4096];
  char prompt_text[16384];
  getcwd(path, 4096);
  catch;
  struct Info info;
  details(&info);
  struct utsname details;
  uname(&details);
  catch;
  int start_path = 0;
  if(strncmp(info.home, path, strlen(info.home)) == 0)
  {
    start_path = strlen(info.home) - 1;
    path[start_path] = '~';
  }
  sprintf(prompt_text, "<%s@%s:%s> ", info.uname, details.nodename, path+start_path);
  print(prompt_text);
}

int main(int argc, char *argv[], char * envp[]) {
  int run = 0;
  char cmd[131072]; //128KB Buffer for reading in commands
  while(run == 0)
  {
    prompt();
    ssize_t length = read(0, cmd, 131072);
    cmd[length-1] = '\0';
    int pid = fork();
    if(pid == 0)
    {
      return 0;
    }
    else
    {
      wait(&pid);
    }
  }
  return 0;
}
