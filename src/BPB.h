#ifndef _BPB_H
#define _BPB_H

void ReadBPB(FILE*);
void PrintBPB();
int LocateDir(int);
int getRootClustNum();
int getRsvdSecCnt();
int getBytsPerSec();
int getSecPerClus();

#endif
