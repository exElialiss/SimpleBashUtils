#include "grep.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    char *temp = NULL;
    FlagsGrep flags = {0};
    for (int InputArgs = FlagsIdentGrep(argc, argv, &flags, &temp);
         InputArgs < argc; InputArgs++) {
      openread_file(argv[InputArgs], flags, temp);
    }
    if (temp != NULL) {
      free(temp);
    }
  }
  return 0;
}

//идентификация флагов
int FlagsIdentGrep(int argc, char **argv, FlagsGrep *flags, char **templ) {
  for (int currentFlag = 0; currentFlag != -1;
       currentFlag = getopt(argc, argv, "e:ivclnhsf:o")) {
    switch (currentFlag) {
      case 'e':
        flagE(flags, templ, optarg);
        break;
      case 'i':
        flags->flag_i = 1;
        break;
      case 'v':
        flags->flag_v = 1;
        break;
      case 'c':
        flags->flag_c = 1;
        break;
      case 'l':
        flags->flag_l = 1;
        break;
      case 'n':
        flags->flag_n = 1;
        break;
      case 'h':
        flags->flag_h = 1;
        break;
      case 's':
        flags->flag_s = 1;
        break;
      case 'f':
        flagF(flags, templ, optarg);
        break;
      case 'o':
        flags->flag_o = 1;
        break;
    }
  }
  if ((flags->flag_e || flags->flag_f) == 0) {
    PatternBuf(templ, argv[optind]);
    ++optind;
  }
  disableFlagO(flags, argc, optind);
  return optind;
}

//
void openread_file(char *path, FlagsGrep flags, char *templ) {
  FILE *file = fopen(path, "r");

  if (file != NULL) {
    for (char symbol = '0'; (symbol = getc(file)) != EOF;) {
      ++flags.numberLine;
      int length = 0;
      char *line = calloc(256, 1);

      if (line == NULL) {
        printf("Can't allocate memory\n");
        exit(0);
      }

      for (line[length] = '\0'; symbol != EOF && symbol != '\n';
           symbol = getc(file)) {
        line[length] = symbol;
        line[++length] = '\0';

        if (length % 255 == 0) {
          line = realloc(line, length + 256);

          if (line == NULL) {
            printf("Can't allocate memory\n");
            exit(0);
          }
        }
      }

      pattern_search(line, &flags, templ, path);
      free(line);
    }
    flagLC(flags, path);
    fclose(file);
  } else if (flags.flag_s == 0) {
    fprintf(stderr, ERROR, path);
  }
}

//компилируеv регулярное выражение из шаблона
void pattern_search(char *line, FlagsGrep *flags, char *templ, char *path) {
  regex_t regex;

  if (regcomp(&regex, templ, flags->flag_i ? REG_ICASE : REG_EXTENDED) == 0) {
    if (regexec(&regex, line, 0, NULL, 0) == flags->flag_v) {
      flags->countMatches++;

      if ((flags->flag_c || flags->flag_l) == 0) {
        if (flags->countFiles > 1 && !(flags->flag_h)) {
          printf("%s:", path);
        }

        if (flags->flag_n) {
          printf("%i:", flags->numberLine);
        }

        if (!flags->flag_o) {
          printf("%s\n", line);
        } else {
          flagO(regex, line, *flags, path);
        }
      }
    }
    regfree(&regex);
  }
}

void flagF(FlagsGrep *flags, char **templ, char *optarg) {
  FILE *file = fopen(optarg, "r");
  if (file != NULL) {
    for (char sym = '0'; (sym = getc(file)) != EOF;) {
      int length = 0;
      char *line = calloc(256, 1);

      if (line == NULL) {
        printf("Can't allocate memory\n");
        exit(0);
      }

      for (line[length] = '\0'; sym != EOF && sym != '\n'; sym = getc(file)) {
        line[length] = sym;
        line[length += 1] = '\0';
        if (length % 255 == 0) line = (char *)realloc(line, length + 256);

        if (line == NULL) {
          printf("Can't allocate memory\n");
          exit(0);
        }
      }

      if (!(flags->flag_e || flags->flag_f)) {
        flags->flag_f = PatternBuf(templ, line);
      } else {
        pattern_add_buf(templ, line);
      }

      free(line);
    }
    fclose(file);
  } else if (flags->flag_s == 0) {
    fprintf(stderr, ERROR, optarg);
  }
}

void flagE(FlagsGrep *flags, char **templ, char *optarg) {
  if (!(flags->flag_e || flags->flag_f)) {
    flags->flag_e = PatternBuf(templ, optarg);
  } else {
    pattern_add_buf(templ, optarg);
  }
}

void flagLC(FlagsGrep flags, char *path) {
  if (flags.flag_c) {
    if (flags.flag_l) {
      if (flags.countFiles > 1 && flags.countMatches != 0) {
        printf("%s\n", path);
      }
    } else {
      if (flags.countFiles > 1 && !flags.flag_h) {
        printf("%s:", path);
      }
      printf("%i\n", flags.countMatches);
    }
  }
  if (flags.flag_l && flags.countMatches) {
    printf("%s\n", path);
  }
}

void flagO(regex_t regex, char *line, FlagsGrep flags, char *path) {
  int countMatchesInLine = 0;

  while (regexec(&regex, line, 0, NULL, 0) == flags.flag_v) {
    int start = 0;
    int end = strlen(line);
    char *buff = calloc(strlen(line) + 1, 1);

    if (countMatchesInLine > 0 && flags.countFiles > 1 && !flags.flag_h) {
      printf("%s:", path);
    }

    if (buff == NULL) {
      printf("Can't allocate memory\n");
      exit(0);
    }
    strcpy(buff, line);

    while (regexec(&regex, buff, 0, NULL, 0) == 0) {
      end--;
      buff[strlen(buff) - 1] = 0;
    }
    buff[strlen(buff)] = line[strlen(buff)];

    while (regexec(&regex, buff, 0, NULL, 0) == 0 && strlen(buff) > 0) {
      int j = 0;

      while (buff[j] != 0) {
        buff[j] = buff[j + 1];
        j++;
      }
      start++;
    }
    start--;
    int buffSize = strlen(buff);

    while (buffSize != 0) {
      buff[buffSize] = buff[buffSize - 1];
      buffSize--;
    }
    buff[0] = line[start];
    printf("%s\n", buff);
    free(buff);
    buffSize = start + 1;

    while (line[buffSize] != 0) {
      line[buffSize - start - 1] = line[buffSize];
      buffSize++;
    }
    line[buffSize - start - 1] = 0;
    ++countMatchesInLine;
  }
}

//Если с флагом -о вводятся флаги -l,-v,-c флаг -о сделаем не активным
void disableFlagO(FlagsGrep *flags, int argc, int optind) {
  if (flags->flag_o && (flags->flag_l || flags->flag_v || flags->flag_c))
    flags->flag_o = 0;
  flags->countFiles = argc - optind;
}

int PatternBuf(char **templ, char *optarg) {
  *templ = calloc(strlen(optarg) + 1, 1);

  if (templ == NULL) {
    printf("Can't allocate memory\n");
    exit(0);
  }
  strcpy(*templ, optarg);
  return templ ? 1 : 0;
}

void pattern_add_buf(char **templ, char *optarg) {
  *templ = realloc(*templ, strlen(*templ) + strlen(optarg) + 2);

  if (templ == NULL) {
    printf("Can't allocate memory\n");
    exit(0);
  }

  if (*templ) {
    strcat(strcat(*templ, "|"), optarg);
  }
}