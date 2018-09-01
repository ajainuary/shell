#include "shell.h"

int main(void) {
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
