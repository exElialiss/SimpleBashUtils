#include "grep.h"


int FlagsIdentGrep(int argc, char **argv, FlagsGrep *flags,
                            char **templ) {
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