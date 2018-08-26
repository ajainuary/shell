#include <errno.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>
#define catch if (errno != 0){ \
    write(1, "Error: ", 7);    \
    write(1, strerror(errno), strlen(strerror(errno))); write(1, "\n", 1); }
#define MAX_READ 1000000
int main() {
  int list = open("/etc/passwd", O_RDONLY);
  catch;
  char buffer[MAX_READ + 1];
  ssize_t sz = read(list, buffer, MAX_READ);
  catch;
  // buffer[sz] = '\0';
  catch;
  int o_uid = 1000;
  for (char *s = strtok(buffer, "\n"); s != NULL; s = strtok(NULL, "\n")) {
    char *name = strtok(NULL, ":");
    strtok(NULL, ":");  // Cycle through the password
    char *uid = strtok(NULL, ":");
    if (uid != NULL && atoi(uid) == o_uid) {
      strtok(NULL, ":");  // Cycle through Group ID
      strtok(NULL, ":");  // Cycle through extra info
      char *home = strtok(NULL, ":");
      printf("%s\n%d\n%s\n", name, atoi(uid), home);
      break;
    }
  }
  return 0;
}