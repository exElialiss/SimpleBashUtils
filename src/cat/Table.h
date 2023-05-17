#ifndef CAT_CATFLAGS_H
#define CAT_CATFLAGS_H
#include <stdio.h>

#include "../common/Flags.h"

void Table(const char *table[static 256]);
void PrintEndl(const char *table[static 256]);
void PrintTab(const char *table[static 256]);
void PrintNonPrinting(const char *table[static 256]);
void Cat(int argc, char *argv[], Flags flags, const char *table[static 256]);
void ProcessFile(FILE *file, Flags flags, const char *table[static 256]);
#endif