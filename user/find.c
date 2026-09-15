#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "user/user.h"

int matchhere(char *re, char *text);
int matchstar(int c, char *re, char *text);

int
match(char *re, char *text)
{
  if (re[0] == '^')
    return matchhere(re + 1, text);
  do {
    if (matchhere(re, text))
      return 1;
  } while (*text++ != '\0');
  return 0;
}

int
matchhere(char *re, char *text)
{
  if (re[0] == '\0')
    return 1;
  if (re[1] == '*')
    return matchstar(re[0], re + 2, text);
  if (re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if (*text != '\0' && (re[0] == '.' || re[0] == *text))
    return matchhere(re + 1, text + 1);
  return 0;
}

int
matchstar(int c, char *re, char *text)
{
  do {
    if (matchhere(re, text))
      return 1;
  } while (*text != '\0' && (*text++ == c || c == '.'));
  return 0;
}

void
runcmd(char **cmdv, int cmdc, char *file)
{
  char *args[MAXARG];
  int i;

  for (i = 0; i < cmdc; i++)
    args[i] = cmdv[i];
  args[i++] = file;
  args[i] = 0;

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "find: fork failed\n");
    return;
  }
  if (pid == 0) {
    exec(args[0], args);
    fprintf(2, "find: exec %s failed\n", args[0]);
    exit(1);
  }
  wait(0);
}

void
find(char *path, char *name, char **execv, int execc)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  char *base = path;
  for (char *q = path; *q; q++)
    if (*q == '/')
      base = q + 1;

  if (match(name, base)) {
    if (execv)
      runcmd(execv, execc, path);
    else
      printf("%s\n", path);
  }

  if (st.type == T_DIR) {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      printf("find: path too long\n");
      close(fd);
      return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, name, execv, execc);
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "usage: find dir name [-exec cmd ...]\n");
    exit(1);
  }

  char **execv = 0;
  int execc = 0;

  if (argc > 3) {
    if (strcmp(argv[3], "-exec") != 0) {
      fprintf(2, "usage: find dir name [-exec cmd ...]\n");
      exit(1);
    }
    execv = &argv[4];
    execc = argc - 4;
  }

  find(argv[1], argv[2], execv, execc);
  exit(0);
}
