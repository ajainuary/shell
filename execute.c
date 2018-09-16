#include "shell.h"
// Signal Handler called when a child process terminates
void sig_child_process_terminated(int sig) {
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) != -1)
    if (WIFEXITED(status))
      printf("Process with pid %d exited normally\n", pid);
    else
      printf("Process with pid %d exited with status %d\n", pid, status);
  if (errno == ECHILD) signal(SIGCHLD, SIG_DFL), errno = 0;
  return;
}

void execute_cmd() {
  int i = 0;
  if (_is_background == 0) {
    pid_t pid = fork();
    if (pid == 0) {
      if (execvp(arg[0], arg) == -1) {
        printf("Wrong Command\n");
        exit(0);
      }
    } else {
      wait(NULL);
    }
  } else {
    pid_t pid = fork();
    if (pid == 0) {
      if (execvp(arg[0], arg) == -1) {
        printf("Wrong Command\n");
        return;
      }
    } else {
      signal(SIGCHLD, sig_child_process_terminated);
      printf("Process with pid %d running in background\n", pid);
    }
  }
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
  if (strncmp(exe, home, length) == 0) {
    exe[length - 1] = '~';
    start = length - 1;
  }
  printf(
      "pid -- %d\n\nProcess Status -- %c\nMemory - %lu\nExecutable Path - "
      "%s\n",
      pid, state, vsize, exe + start);
  return;
}