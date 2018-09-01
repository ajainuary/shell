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
#define _XOPEN_SOURCE
#include <time.h>
#include <unistd.h>
#define catch if (errno != 0){write(1, "Error: ", 7); write( \
    1, strerror(errno), strlen(strerror(errno)));            \
                              write(1, "\n", 1); errno = 0; }

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

void expansion() {
  for (int i = 0; i < argcount; ++i) {
    if (arg[i][0] == '~') {
      char *new = malloc((strlen(home) + strlen(arg[i]) + 3) * sizeof(char));
      strcpy(new, home);
      int len = strlen(home);
      if (arg[i][1] != '/') {
        new[len] = '/';
        new[len + 1] = '\0';
      }
      strcat(new, arg[i] + 1);
      free(arg[i]);
      arg[i] = new;
    }
  }
  return;
}

void execute_cmd() {
  int pid;
  pid = fork();
  int i = 0;
  int bg = 0;
  for (i = 0; i < argcount && arg[i][0] != '&'; ++i)
    ;
  if (i < argcount) {
    free(arg[i]);
    arg[i] = NULL;
    bg = 1;
  }
  if (pid == 0 && bg == 1) {
    int monitor_id = fork();
    if (monitor_id == 0) {
      if (execvp(arg[0], arg) == -1) {
        printf("Wrong Command\n");
        return;
      }
    } else {
      printf("%d running in background\n", monitor_id);
      int tmp = monitor_id;
      wait(NULL);
      printf("\n%d completed successfully\n", tmp);
      return;
    }
  } else if (pid == 0) {
    if (execvp(arg[0], arg) == -1) {
      printf("Wrong Command\n");
      catch;
      return;
    }
  } else if (bg == 0)
    wait(&pid);
  if (bg == 1) sleep(1);
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
  optind = 0;
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

void pinfo() {
  int pid;
  if (argcount == 1)
    pid = getpid();
  else
    pid = atoi(arg[1]);
  char proc_path[4096];
  sprintf(proc_path, "/proc/%d/stat", pid);
  int stat = open(proc_path, O_RDONLY);
  catch;
  char buffer[4096];
  read(stat, buffer, 4096);
  catch;
  char state;
  char comm[256];
  int ppid, pgrp, session, tty_nr, tpgid;
  unsigned int flags;
  unsigned long int minflt, cminflt, majflt, cmajflt, utime, vsize, stime;
  long int itrealvalue, cutime, cstime, priority, nice, num_threads;
  unsigned long long int starttime;
  sscanf(buffer,
         "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld "
         "%ld %ld %llu %lu",
         &pid, comm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid, &flags,
         &minflt, &cminflt, &majflt, &cmajflt, &utime, &stime, &cutime, &cstime,
         &priority, &nice, &num_threads, &itrealvalue, &starttime, &vsize);
  close(stat);
  sprintf(proc_path, "/proc/%d/exe", pid);
  char exe[4096];
  int len = readlink(proc_path, exe, 4096);
  exe[len] = '\0';
  printf(
      "pid -- %d\n\nProcess Status -- %c\nMemory - %lu\nExecutable Path - %s\n",
      pid, state, vsize, exe);
  return;
}

void remindme() {
  if (argcount < 3) {
    printf("Invalid Usage\n");
    return;
  }
  int time = atoi(arg[1]);
  char msg[4096];
  int t = 0;
  for (int i = 2; i < argcount; ++i) {
    for (int j = 0; arg[i][j] != '\0'; ++j)
      if (arg[i][j] != '"') msg[t++] = arg[i][j];
    msg[t++] = ' ';
  }
  msg[t] = '\0';
  int pid = fork();
  if (pid == 0) {
    sleep(time);
    printf("\nReminder : %s\n", msg);
  }
  return;
}

void clock_rtc() {
  int rtc = open("/proc/driver/rtc", O_RDONLY);
  char buffer[1024];
  read(rtc, buffer, 1024);
  strtok(buffer, ":");
  char *time_gmt_tmp = strtok(NULL, "\n");
  char time_gmt[256];
  strcpy(time_gmt, time_gmt_tmp + 1);
  strtok(NULL, ":");  // Cycle through label
  char *date_gmt_tmp = strtok(NULL, "\n");
  char date_gmt[256];
  strcpy(date_gmt, date_gmt_tmp + 1);
  char buffer_time[512];
  strcpy(buffer_time, date_gmt);
  int len = strlen(buffer_time);
  buffer_time[len] = ' ';
  buffer_time[len + 1] = '\0';
  strcat(buffer_time, time_gmt);
  printf("%s\n", buffer_time);
  return;
}

void clock_wrapper() {
  extern char *optarg;
  extern int optind;
  int c;
  int time = 1, step = 1;
  optind = 0;
  while ((c = getopt(argcount, arg, "n:t:")) != -1) {
    switch (c) {
      case 'n':
        time = atoi(optarg);
        break;
      case 't':
        step = atoi(optarg);
        break;
      case '?':
        printf("Invalid Flag\n");
        break;
    }
  }
  for (int i = 0; i < time; i += step) {
    clock_rtc();
    sleep(step);
  }
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
    expansion();
    if (argcount == 0) continue;
    if (strncmp(arg[0], "cd", 2) == 0) {
      cd();
    } else if (strncmp(arg[0], "ls", 2) == 0) {
      ls();
    } else if (strncmp(arg[0], "pwd", 3) == 0) {
      pwd();
    } else if (strncmp(arg[0], "echo", 4) == 0) {
      echo();
    } else if (strncmp(arg[0], "pinfo", 5) == 0) {
      pinfo();
    } else if (strncmp(arg[0], "remindme", 8) == 0) {
      remindme();
    } else if (strncmp(arg[0], "clock", 5) == 0) {
      clock_wrapper();
    } else {
      execute_cmd();
    }
    free_args();
    fflush(stdout);
  }
  return 0;
}
