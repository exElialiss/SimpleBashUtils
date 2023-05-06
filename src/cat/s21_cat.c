#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "CatFlag.h"
typedef struct {
  bool numberNonBlank;
  bool markEndl;
  bool numberAll;
  bool squeeze;
  bool tab;
  bool printNonPrintble;

} Flags;

Flags CatReadFlags(int argc, char *argv[]) {
  struct option longOption[] = {{"number-non-blank", 0, NULL, 'b'},
                                {"number", 0, NULL, 'n'},
                                {"squeeze-blank", 0, NULL, 's'},
                                {NULL, 0, NULL, 0}};

  int currentFlag = getopt_long(argc, argv, "bevEnstT", longOption, NULL);
  Flags flags = {false, false, false, false, false, false};
  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "bevEnstT", longOption, NULL)) {
    switch (currentFlag) {
      break;
      case 'b':
        flags.numberNonBlank = true;
        break;
      case 'e':
        flags.markEndl = true;
      case 'v':
        flags.printNonPrintble = true;
        break;
      case 'E':
        flags.markEndl = true;
        break;
      case 'n':
        flags.numberAll = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
        flags.printNonPrintble = true;
        break;
      case 'T':
        flags.tab = true;
    }
  }
  return flags;
}

void CatFile(FILE *file, Flags flags, const char *table[static 256]) {
  int c = 0;
  int last;
  bool sqz = false;
  int lineno = 0;
  last = '\n';
  (void)flags;
  while (fread(&c, 1, 1, file) > 0) {
    if (last == '\n') {
      if (flags.squeeze && c == '\n') {
        if (sqz) continue;
        sqz = true;
      } else
        sqz = false;
      if (flags.numberNonBlank) {
        if (c != '\n') printf("%6i\t", ++lineno);
      } else if (flags.numberAll) {
        printf("%6i\t", ++lineno);
      }
    }
    if (!*table[c])
      printf("%c", '\0');
    else {
      printf("%s", table[c]);
    }
    last = c;
  }
}

void Cat(int argc, char *argv[], Flags flags, const char *table[static 256]) {
  for (char **filename = &argv[1], **end = &argv[argc]; filename != end;
       ++filename) {
    if (**filename == '-') continue;
    FILE *file = fopen(*filename, "r");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    CatFile(file, flags, table);
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = CatReadFlags(argc, argv);
  const char *table[256];
  catSetTable(table);
  if (flags.markEndl) CatSetEndl(table);
  if (flags.tab) CatSetTab(table);
  if (flags.printNonPrintble) CatSetNonPrintble(table);
  Cat(argc, argv, flags, table);
}