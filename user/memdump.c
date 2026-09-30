#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data, int len);

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("Example 1:\n");
    int a[2] = {61810, 2026};
    memdump("ii", (char *)a, sizeof(a));

    printf("Example 2:\n");
    memdump("S", "a string", sizeof("a string"));

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *)&s, sizeof(s));

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char *)&example, sizeof(example));

    printf("Example 5:\n");
    memdump("sccccc", (char *)&example, sizeof(example));
  } else if (argc == 2) {
    char data[512];
    int n = 0;
    while (n < sizeof(data)) {
      int nn = read(0, data + n, sizeof(data) - n);
      if (nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data, n);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data, int len)
{
  int pos = 0;
  for (int i = 0; fmt[i]; i++) {
    char f = fmt[i];
    int need = f == 'i'   ? 4
               : f == 'p' ? 8
               : f == 'h' ? 2
               : f == 'c' ? 1
               : f == 's' ? 8
                          : 0;

    if (f != 'S' && pos + need > len) {
      printf("memdump: not enough data for '%c'\n", f);
      return;
    }

    if (f == 'i') {
      int v;
      memmove(&v, data + pos, 4);
      printf("%d\n", v);
      pos += 4;
    } else if (f == 'p') {
      uint64 v;
      memmove(&v, data + pos, 8);
      printf("%lx\n", v);
      pos += 8;
    } else if (f == 'h') {
      short v;
      memmove(&v, data + pos, 2);
      printf("%d\n", v);
      pos += 2;
    } else if (f == 'c') {
      printf("%c\n", data[pos]);
      pos += 1;
    } else if (f == 's') {
      char *p;
      memmove(&p, data + pos, 8);
      printf("%s\n", p);
      pos += 8;
    } else if (f == 'S') {
      while (pos < len && data[pos])
        printf("%c", data[pos++]);
      printf("\n");
    }
  }
}
