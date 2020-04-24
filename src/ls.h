#ifndef _LS_H
#define _LS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct DIRENTRY {
	unsigned char	DIR_Name[11],
			DIR_Attributes;
	unsigned int	DIR_FstClus,
			DIR_FileSize;
} DIRENTRY;

void ls(const char*, FILE*, unsigned int);
void PrintFileContents(int, FILE*);
unsigned int NextCluster(unsigned int, FILE*);
void format(DIRENTRY*, unsigned char*);

#endif
