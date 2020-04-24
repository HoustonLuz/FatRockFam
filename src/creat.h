#ifndef _CREAT_H
#define _CREAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ls.h"
#include "BPB.h"

void creat(FILE*, unsigned int, char*);
unsigned int findFirstFreeCluster(FILE* f);
void addDirEnt(FILE*, unsigned int, DIRENTRY*);

void mkdir(FILE*, unsigned int, char*);

#endif
