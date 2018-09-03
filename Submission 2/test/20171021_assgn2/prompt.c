#include "shell.h"

int main(void) {
  int run = 0;
  // Set the home variable
  getcwd(home, 4096);
  catch;
  while (run == 0) {
    prompt();
    read_cmd();
    for (int i = 0; commands[i] != NULL; ++i) {
      expansion(i);
      if (argcount == 0) continue;
      if (strncmp(arg[0], "cd", 2) == 0) {
        cd();
      } else if (strcmp(arg[0], "ls\0") == 0) {
        ls();
      } else if (strcmp(arg[0], "pwd\0") == 0) {
        pwd();
      } else if (strcmp(arg[0], "echo\0") == 0) {
        echo();
      } else if (strcmp(arg[0], "pinfo\0") == 0) {
        pinfo();
      } else if (strcmp(arg[0], "remindme\0") == 0) {
        remindme();
      } else if (strcmp(arg[0], "clock\0") == 0) {
        clock_wrapper();
      } else {
        execute_cmd();
      }
      free_args();
      fflush(stdout);
    }
  }
  return 0;
}
