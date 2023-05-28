#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR "grep: %s: No such file or directory\n"

typedef struct {
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int flag_h;
  int flag_s;
  int flag_o;
  int flag_e;
  int flag_f;
  int countFiles;
  int numberLine;
  int countMatches;
} FlagsGrep;

void openread_file(char *name, FlagsGrep flags, char *tmpl);
int FlagsIdentGrep(int argc, char **argv, FlagsGrep *flags, char **templ);
void pattern_search(char *str, FlagsGrep *FlagGrep, char *tmpl, char *name);
void flagF(FlagsGrep *flags, char **templ, char *optarg);
void flagE(FlagsGrep *flags, char **templ, char *optarg);
void flagO(regex_t my_regex, char *str, FlagsGrep flags, char *path);
void flagLC(FlagsGrep flags, char *path);
void disableFlagO(FlagsGrep *flags, int argc, int optind);
int PatternBuf(char **str, char *optarg);
void pattern_add_buf(char **str, char *optarg);

#endif