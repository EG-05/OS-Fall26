#include "kernel/types.h"
#include "user/user.h"
int main(void) {
  printf("--- before ---\n");
  pgprint();
  char *p = sbrk(1);
  printf("--- after sbrk(1), p=%p ---\n", p);
  pgprint();
  exit(0);
}
