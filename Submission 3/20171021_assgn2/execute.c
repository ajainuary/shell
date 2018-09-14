#include "shell.h"

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
      int tmp;
      wait(&tmp);
      if(tmp == 0)
        printf("\n%d completed successfully\n", monitor_id);
      else
        printf("\nCommand %d exited abnormally\n", monitor_id);
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
  int start = 0;
  int length = strlen(home);
  if(strncmp(exe, home, length) == 0)
  {
    exe[length-1] = '~';
    start = length-1;
  }
  printf(
      "pid -- %d\n\nProcess Status -- %c\nMemory - %lu\nExecutable Path - %s\n",
      pid, state, vsize, exe+start);
  return;
}