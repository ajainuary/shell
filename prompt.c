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
#define catch if (errno != 0){ \
    write(1, "Error: ", 7);    \
    write(1, strerror(errno), strlen(strerror(errno))); write(1, "\n", 1); }

#define print(x) write(1, x, strlen(x))
#define MAX_READ 1000000
#define WHITESPACE " \t\r\a\n"
char home[4096];
char *arg[32767];
char argcount;
struct Info {
  int uid;
  char uname[4096];
};

void get_user(int uid, char *uname) {
  int list = open("/etc/passwd", O_RDONLY);
  catch;
  char buffer[MAX_READ + 1];
  ssize_t sz = read(list, buffer, MAX_READ);
  catch;
  for (char *s = strtok(buffer, "\n"); s != NULL; s = strtok(NULL, "\n")) {
    char *name = strtok(NULL, ":");
    strtok(NULL, ":");  // Cycle through the password
    char *this_uid = strtok(NULL, ":");
    if (this_uid != NULL && atoi(this_uid) == uid) {
      strtok(NULL, ":");  // Cycle through Group ID
      strtok(NULL, ":");  // Cycle through extra info
      // char *home = strtok(NULL, ":");
      strcpy(uname, name);
      // strcpy(instance->home, home);
      break;
    }
  }
  return;
}

void free_args() {
  for (int i = 0; i < argcount; ++i) {
    free(arg[i]);
    arg[i] = NULL;
  }
}

void prompt() {
  char path[4096];
  char prompt_text[16384];
  getcwd(path, 4096);
  catch;
  char name[4096];
  get_user(getuid(), name);
  struct utsname details;
  uname(&details);
  catch;
  int start_path = 0;
  if (strncmp(home, path, strlen(home)) == 0) {
    start_path = strlen(home) - 1;
    path[start_path] = '~';
  }
  sprintf(prompt_text, "<%s@%s:%s> ", name, details.nodename,
          path + start_path);
  print(prompt_text);
}

void read_cmd() {
  char buffer[131072];
  memset(buffer, 0, 131072);
  int i = 0;
  while ((buffer[i] = fgetc(stdin)) != EOF) {
    if (buffer[i] == '\n' && buffer[i - 1] != '\\')
      break;  // Proposed feature : Multiline Input
    ++i;
  }
  if (i == 0) {
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
    chdir(home);  // Handle relative paths
    catch;
  } else {
    chdir(arg[1]);
    catch;
  }
}

void perms(int perm[][3], struct stat *test) {
  if ((test->st_mode & S_IRUSR) != 0)
    perm[0][0] = 1;
  else
    perm[0][0] = 0;
  if ((test->st_mode & S_IWUSR) != 0)
    perm[0][1] = 1;
  else
    perm[0][1] = 0;
  if ((test->st_mode & S_IXUSR) != 0)
    perm[0][2] = 1;
  else
    perm[0][2] = 0;
  if ((test->st_mode & S_IRGRP) != 0)
    perm[1][0] = 1;
  else
    perm[1][0] = 0;
  if ((test->st_mode & S_IWGRP) != 0)
    perm[1][1] = 1;
  else
    perm[1][1] = 0;
  if ((test->st_mode & S_IXGRP) != 0)
    perm[1][2] = 1;
  else
    perm[1][2] = 0;
  if ((test->st_mode & S_IROTH) != 0)
    perm[2][0] = 1;
  else
    perm[2][0] = 0;
  if ((test->st_mode & S_IWOTH) != 0)
    perm[2][1] = 1;
  else
    perm[2][1] = 0;
  if ((test->st_mode & S_IXOTH) != 0)
    perm[2][2] = 1;
  else
    perm[2][2] = 0;
  return;
}

void ls() {
  extern char *optarg;
  extern int optind;
  int c, flag_long = 0, flag_hidden = 0, flag_path = 0;
  while ((c = getopt(argcount, arg, "al")) != -1) {
    switch (c) {
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
  char path[4096];
  if (optind < argcount) flag_path = 1;
  DIR *p;
  if (flag_path == 1) {
    p = opendir(arg[optind]);
    catch;
  } else {
    getcwd(path, 4096);
    p = opendir(path);
    catch;
  }
  struct dirent *file;
  while ((file = readdir(p))) {
    int print = 1;
    if (flag_hidden == 0 && file->d_name[0] == '.') print = 0;
    if (print == 1) {
      if (flag_long == 1) {
        struct stat details;
        char tmp[4096];
        strcpy(tmp, (flag_path == 1) ? arg[optind] : path);
        int len = strlen(tmp);
        tmp[len] = '/';
        tmp[len + 1] = '\0';
        strcat(tmp, file->d_name);
        lstat(tmp, &details);
        int perm[3][3];
        perms(perm, &details);
        char user[4096];
        get_user(details.st_uid, user);
        char group[4096];
        get_user(details.st_gid, group);
        char time[256];
        ctime_r(&details.st_mtime, time);
        int length = strlen(time);
        int i;
        for (i = 0; time[i] != '\n'; ++i)
          ;
        time[i] = '\0';
        printf("%c%c%c%c%c%c%c%c%c %3lu %12s %12s %10ld %s %s\n",
               (perm[0][0] == 0) ? '-' : 'r', (perm[0][1] == 0) ? '-' : 'w',
               (perm[0][2] == 0) ? '-' : 'x', (perm[1][0] == 0) ? '-' : 'r',
               (perm[1][1] == 0) ? '-' : 'w', (perm[1][2] == 0) ? '-' : 'x',
               (perm[2][0] == 0) ? '-' : 'r', (perm[2][1] == 0) ? '-' : 'w',
               (perm[2][2] == 0) ? '-' : 'x', details.st_nlink, user, group,
               details.st_size, time, file->d_name);
      } else
        printf("%s ", file->d_name);
    }
  }
  printf("\n");
  optind = 1;
  return;
}

void echo() {
  for (int i = 1; i < argcount; ++i) printf("%s ", arg[i]);
  printf("\n");
  return;
}

int main(int argc, char *argv[], char *envp[]) {
  int run = 0;
  // Set the home variable
  getcwd(home, 4096);
  catch;
  while (run == 0) {
    prompt();
    read_cmd();
    if (argcount == 0) continue;
    if (strncmp(arg[0], "cd", 2) == 0) {
      cd();
    } else if (strncmp(arg[0], "ls", 2) == 0) {
      ls();
    } else if (strncmp(arg[0], "pwd", 3) == 0) {
      pwd();
    } else if (strncmp(arg[0], "echo", 4) == 0) {
      echo();
    }
    free_args();
    fflush(stdout);
  }
  return 0;
}
