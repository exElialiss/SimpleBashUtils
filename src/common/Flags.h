#ifndef FLAGS_IDENT_H
#define FLAGS_IDENT_H

typedef struct {
  int flag_b;
  int flag_e;
  int flag_n;
  int flag_s;
  int flag_t;
  int flag_v;

} Flags;

Flags FlagsIdent(int argc, char* argv[], Flags* flags);

#endif