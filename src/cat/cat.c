#include "cat.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// идентификация флагов
Flags FlagsIdent(int argc, char *argv[], Flags *flags) {
  struct option longOption[] = {
      {"number-nonblank", 0, 0, 'b'},  // структура массивов с длинными флагами
      {"number", 0, 0, 'n'},  // и соответсвующими ими короткими флагами
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0}};
  // определение введенного флага
  for (int opt = getopt_long(argc, argv, "bevEnstT", longOption, 0);
       opt != -1;
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