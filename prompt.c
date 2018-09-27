#include "shell.h"
char *commands[256];
char home[4096];
char *arg[32767];
char argcount;
int main(void) {
  int run = 0;
  // Set the home variable
  getcwd(home, 4096);
  while (run == 0) {
    prompt();
    read_cmd();
    for (int i = 0; commands[i] != NULL; ++i) {
      char *piped[256];
      int pipes[256][2];
      int j = 0;
      for (char *s = strtok(commands[i], "|"); s != NULL;
           ++j, s = strtok(NULL, "|")) {
        piped[j] = malloc(strlen(s) + 1);
        strcpy(piped[j], s);
      }
      piped[j] = NULL;
      if (j > 1) {
        // First pipe
        pipe(pipes[0]);
        free_args();
        int leader = fork();
        if (leader == 0) {
          // close(1);
          dup2(pipes[0][1], 1);  // Redirect my output to the pipe
          close(pipes[0][0]);
          interpret(piped[0]);
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
          } else if (strcmp(arg[0], "setenv\0") == 0) {
            Mysetenv();
          } else if (strcmp(arg[0], "unsetenv\0") == 0) {
            Myunsetenv();
          } else {
            fflush(stdout);
            if (execvp(arg[0], arg) == -1) {
              printf("Wrong Command\n");
              exit(0);
            }
          }
          // close(1);
          exit(0);
        }
        close(pipes[0][1]);  // Close unused fd (Read End)
        setpgid(leader, leader);
        // close(pipes[0][1]);
        for (int k = 1; k < j - 1; ++k) {
          pipe(pipes[k]);
          int middling = fork();
          if (middling == 0) {
            dup2(pipes[k][1], 1);  // Redirect my output to the pipe
            close(pipes[k][0]);
            dup2(pipes[k - 1][0], 0);  // Redirect my input to pipe
            close(pipes[k - 1][1]);
            interpret(piped[k]);
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
              // fflush(stdout);
              if (execvp(arg[0], arg) == -1) {
                printf("Wrong Command\n");
                exit(0);
              }
            }
            // close(pipes[k - 1][0]);  // Close unused fd (Read End)
            // close(pipes[k][1]);      // close unused fd (Write End)
            exit(0);
          }
            close(pipes[k - 1][0]);  // Close unused fd (Read End)
            close(pipes[k][1]);      // close unused fd (Write End)
          setpgid(middling, leader);
        }

        int laggard = fork();  // last of the real ones
        free_args();
        if (laggard == 0) {
          dup2(pipes[j - 2][0], 0);  // Redirect my input to pipe
          close(pipes[j - 2][1]);
          interpret(piped[j - 1]);
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
          } else if (strcmp(arg[0], "setenv\0") == 0) {
            Mysetenv();
          } else if (strcmp(arg[0], "unsetenv\0") == 0) {
            Myunsetenv();
          } else {
            if (execvp(arg[0], arg) == -1) {
              printf("Wrong Command\n");
              exit(0);
            }
          }
          // close(pipes[j - 2][1]);  // close unused fd (Write End)
          exit(0);
        }
          close(pipes[j - 2][0]);  // close unused fd (Write End)
        setpgid(laggard, leader);
        // close(pipes[0][0]);
        while (waitpid(-1, NULL, 0) != -1)
          ;

        errno = 0;
      } else {
        free_args();
        // Save state of fd table
        int out, in;
        dup2(1, out);
        dup2(0, in);
        interpret(commands[i]);
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
        } else if (strcmp(arg[0], "setenv\0") == 0) {
          Mysetenv();
        } else if (strcmp(arg[0], "unsetenv\0") == 0) {
          Myunsetenv();
        } else if (strcmp(arg[0], "jobs\0") == 0) {
          jobs();
        } else if (strcmp(arg[0], "fg\0") == 0) {
          fg();
        } else if (strcmp(arg[0], "bg\0") == 0) {
          bg();
        } else if (strcmp(arg[0], "kjob\0") == 0) {
          kjob();
        } else if (strcmp(arg[0], "overkill\0") == 0) {
          overkill();
        } else if (strcmp(arg[0], "quit\0") == 0) {
          return 0;
        } else {
          printf("send\n");
          execute_cmd();
        }
        fflush(stdout);
        // Restore state of fd table
        dup2(out, 1);
        dup2(in, 0);
      }
    }
  }
  return 0;
}
