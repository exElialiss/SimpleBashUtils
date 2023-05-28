
#include "Table.h"
#include "cat.h"

int main(int argc, char *argv[]) {
  Flags flags = {0, 0, 0, 0, 0, 0};
  flags = FlagsIdent(argc, argv, &flags);
  const char *table[256];
  Table(table);
  if (flags.flag_e) PrintEndl(table);
  if (flags.flag_t) PrintTab(table);
  if (flags.flag_v) PrintNonPrinting(table);
  Cat(argc, argv, flags, table);
}
Flags FlagsIdent(int argc, char *argv[], Flags *flags) {
  struct option longOption[] = {
      {"number-nonblank", 0, 0, 'b'},  // структура массивов с длинными флагами
      {"number", 0, 0, 'n'},  // и соответсвующими ими короткими флагами
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0}};
  // определение введенного флага
  for (int opt = getopt_long(argc, argv, "bevEnstT", longOption, 0); opt != -1;
       opt = getopt_long(argc, argv, "bevEnstT", longOption, 0)) {
    switch (opt) {
      break;
      case 'b':
        flags->flag_b = 1;
        break;
      case 'e':
        flags->flag_e = 1;
        flags->flag_v = 1;
        break;
      case 'v':
        flags->flag_v = 1;
        break;
      case 'E':
        flags->flag_e = 1;
        break;
      case 'n':
        flags->flag_n = 1;
        break;
      case 's':
        flags->flag_s = 1;
        break;
      case 't':
        flags->flag_t = 1;
        flags->flag_v = 1;
        break;
      case 'T':
        flags->flag_t = 1;
    }
  }
  return *flags;
}

// обработка файла и вывод в консоль
void ProcessFile(FILE *file, Flags flags, const char *table[static 256]) {
  int c = 0;
  int lastSymbol = '\n';
  int sqz = 0;
  int lineNumber = 0;
  (void)flags;
  while (fread(&c, 1, 1, file) > 0) {
    if (lastSymbol == '\n') {
      if (flags.flag_s && c == '\n') {
        if (sqz) continue;
        sqz = 1;
      } else
        sqz = 0;
      if (flags.flag_b) {
        if (c != '\n') printf("%6i\t", ++lineNumber);
      } else if (flags.flag_n) {
        printf("%6i\t", ++lineNumber);
      }
    }
    if (!*table[c])
      printf("%c", '\0');
    else {
      printf("%s", table[c]);
    }
    lastSymbol = c;
  }
}
// поверяет введенные данные, открывает файл для чтения
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
    ProcessFile(file, flags, table);
    fclose(file);
  }
}