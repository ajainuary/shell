#include "shell.h"

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
  if (argcount == 1) {
    chdir(home);
    catch;
  } else if (arg[1][0] == '~') {
    chdir(home);  // Handle relative paths
    catch;
  } else {
    chdir(arg[1]);
    catch;
  }
}

void echo() {
  for (int i = 1; i < argcount; ++i) printf("%s ", arg[i]);
  printf("\n");
  return;
}
