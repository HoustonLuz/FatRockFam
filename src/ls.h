#ifndef _LS_H
#define _LS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct DIRENTRY {
	unsigned char	DIR_Name[11],
			DIR_Attributes,
			DIR_FstClus[4]
			DIR_FileSize[4];
} DIRENTRY;

void ls(const char*, FILE*, unsigned int);
void PrintFileContents(int, FILE*);
unsigned int NextCluster(unsigned int, FILE*);
void format(DIRENTRY*, char*);

#endif
