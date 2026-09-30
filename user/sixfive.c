#include "kernel/types.h"
#include "user/user.h"

char *seps = " -\r\t\n./,";

void
process(int fd)
{
  char c;
  long num = 0;
  int have = 0;

  while (read(fd, &c, 1) == 1) {
    if (c >= '0' && c <= '9') {
      num = num * 10 + (c - '0');
      have = 1;
    } else if (strchr(seps, c)) {
      if (have && (num % 5 == 0 || num % 6 == 0))
        printf("%ld\n", num);
      num = 0;
      have = 0;
    }
  }
  if (have && (num % 5 == 0 || num % 6 == 0))
    printf("%ld\n", num);
}

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    process(0);
  } else {
    for (int i = 1; i < argc; i++) {
      int fd = open(argv[i], 0);
      if (fd < 0) {
        printf("sixfive: cannot open %s\n", argv[i]);
        continue;
      }
      process(fd);
      close(fd);
    }
  }
  exit(0);
}
