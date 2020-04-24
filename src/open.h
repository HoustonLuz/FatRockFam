#ifndef _BPB_H
#define _BPB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ls.h"

void myOpen(FILE*, unsigned int, char*, int);
void myClose(FILE*, unsigned int, const char*);

void listInit();
void listDest();
void listAdd(char*, unsigned int, int);
void listRm(const char*, unsigned int);
void allocMoreOpen();
int locate(const char*, unsigned int);
int permissions(char*, unsigned int);

typedef struct {
	int		size,
			cap;
	char**		names;
	unsigned int*	clus;
	int*		flags;

	//Names and flags are paralel arrays.
	// flags[i] is 1 if r enabled, 2 if w enabled,
	// 3 if both.
} flagList;

#endif
