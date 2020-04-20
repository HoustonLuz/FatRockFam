#ifndef _LS_H
#define _LS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct DIRENTRY {
	unsigned char DIR_Name[11];
	unsigned char DIR_Attributes;
	char Name[11];
	unsigned char END;
} DIRENTRY;

void ls(const char*, FILE*, unsigned int);
void PrintFileContents(int, FILE*);
unsigned int NextCluster(unsigned int, FILE*);

#endif
