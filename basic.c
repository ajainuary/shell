#include "shell.h"

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