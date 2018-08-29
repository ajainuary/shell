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
#include <unistd.h>
#include <dirent.h>
#define catch if (errno != 0){ \
    write(1, "Error: ", 7);    \
    write(1, strerror(errno), strlen(strerror(errno))); write(1, "\n", 1); }

#define print(x) write(1, x, strlen(x))
#define MAX_READ 1000000
#define WHITESPACE " \t\r\a\n\0"
char home[4096];
char *arg[32767];
char argcount;
struct Info {
  int uid;
  char uname[4096];
  // char home[4096];
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
      // char *home = strtok(NULL, ":");
      strcpy(instance->uname, name);
      // strcpy(instance->home, home);
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
  if (strncmp(home, path, strlen(home)) == 0) {
    start_path = strlen(home) - 1;
    path[start_path] = '~';
  }
  sprintf(prompt_text, "<%s@%s:%s> ", info.uname, details.nodename,
          path + start_path);
  print(prompt_text);
}

void read_cmd() {
  char buffer[131072];
  int i = 0;
  while((buffer[i] = fgetc(stdin)) != EOF)
  {
    if(buffer[i] == '\n' && buffer[i-1] != '\\')
      break; //Proposed feature : Multiline Input
    ++i;
  }
  if(i == 0)
  {
    argcount = 0;
    return;
  }
  i = 0;
  for (char *s = strtok(buffer, WHITESPACE); s != NULL && i < 32767;
       s = strtok(NULL, WHITESPACE), ++i) {
    arg[i] = malloc(strlen(s) + 1);
    strcpy(arg[i], s);
  }
  arg[i] = NULL;
  argcount = i;
  return;
}

void pwd() {
  char path[4096];
  getcwd(path, 4096);
  catch;
  int length = strlen(path);
  path[length] = '\n';
  write(1, path, length + 1);
  return;
}

void cd() {
  if (arg[1][0] == '~') {
    chdir(home);
    catch;
  } else {
    chdir(arg[1]);
    catch;
  }
}

void ls() {
  extern char *optarg;
  extern int optind;
  int c, flag_long = 0, flag_hidden = 0, flag_path = 0;
  while((c = getopt(argcount, arg, "al")) != -1)
  {
    switch(c)
    {
      case 'l':
        flag_long = 1;
        break;
      case 'a':
        flag_hidden = 1;
        break;
      case '?':
        printf("Not a valid flag\n");
        break;
    }
  }
  if(optind < argcount)
    flag_path = 1;
  DIR * p;
  if(flag_path == 1)
  {
    p = opendir(arg[optind]);
    catch;
  }
  else
  {
    char path[4096];
    getcwd(path, 4096);
    p = opendir(path);
    catch;
  }
  optind = 1;
  struct dirent * file;
  while((file = readdir(p)))
  {
    if(file->d_name[0] == '.')
    {
      if(flag_hidden == 1)
      {
        printf("%s ", file->d_name);
      }
    }
    else
      printf("%s ", file->d_name);
  }
  printf("\n");
  return;
}

int main(int argc, char *argv[], char *envp[]) {
  int run = 0;
  char cmd[131072];  // 128KB Buffer for reading in commands
  // Set the home variable
  getcwd(home, 4096);
  catch;
  while (run == 0) {
    prompt();
    read_cmd();
    if(argcount == 0)
      continue;
    if (strncmp(arg[0], "cd", 2) == 0) {
      cd();
    } else if (strncmp(arg[0], "ls", 2) == 0) {
      ls();
    } else if (strncmp(arg[0], "pwd", 3) == 0) {
      pwd();
    }
    fflush(stdout);
  }
  return 0;
}