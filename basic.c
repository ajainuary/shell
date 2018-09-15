#include "shell.h"

void free_args() {
  for (int i = 0; arg[i] != NULL; ++i) {
    free(arg[i]);
    arg[i] = NULL;
  }
}

void prompt() {
  char path[4096];
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
  printf("<%s@%s:%s> ", name, details.nodename, path + start_path);
}

void read_cmd() {
  char buffer[131072];
  memset(buffer, 0, 131072);
  for (int i = 0; commands[i] != NULL; ++i) {
    free(commands[i]);
    commands[i] = NULL;
  }
  int i = 0;
  while ((buffer[i] = fgetc(stdin)) != EOF) {
    if (buffer[i] == '\n' && (i == 0 || buffer[i - 1] != '\\')) break;
    ++i;
  }
  if (i == 0) {
    argcount = 0;
    return;
  }
  i = 0;
  for (char *s = strtok(buffer, ";"); s != NULL && i < 32767;
       s = strtok(NULL, ";"), ++i) {
    commands[i] = malloc(strlen(s) + 1);
    strcpy(commands[i], s);
  }
  commands[i] = NULL;
  return;
}

void expansion(int n) {
  free_args();
  if (commands[n] == NULL) {
    printf("NULL\n");
    return;
  }
  int j = 0;
  for (char *s = strtok(commands[n], WHITESPACE); s != NULL && j < 32767;
       s = strtok(NULL, WHITESPACE), ++j) {
    arg[j] = malloc(strlen(s) + 1);
    strcpy(arg[j], s);
  }
  arg[j] = NULL;
  argcount = j;
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