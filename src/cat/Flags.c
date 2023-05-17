#include "Flags.h"

#include <getopt.h>

// идентификация флагов
Flags FlagsIdent(int argc, char* argv[], Flags* flags) {
  struct option longOption[] = {
      {"number-nonblank", 0, 0, 'b'},  // структура массивов с длинными флагами
      {"number", 0, 0, 'n'},  // и соответсвующими ими короткими флагами
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0}};
  // определение введенного флага
  for (int currentFlag = getopt_long(argc, argv, "bevEnstT", longOption, 0);
       currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "bevEnstT", longOption, 0)) {
    switch (currentFlag) {
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