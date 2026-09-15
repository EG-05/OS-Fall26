
#include "kernel/types.h"
#include "user/user.h"

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
