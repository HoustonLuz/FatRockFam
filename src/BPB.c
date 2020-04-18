#include <stdio.h>
#include <stdlib.h>
#include "BPB.h"

struct bpbStruct {
	unsigned int BPB_BytsPerSec;
	unsigned int BPB_SecPerClus;
	unsigned int BPB_RsvdSecCnt;
	unsigned int BPB_NumFATs;
	unsigned int BPB_FATSz32;
	unsigned int BPB_RootClus;
	unsigned int BPB_TotSec32;
};

struct bpbStruct bb;

void ReadBPB(FILE* f) {
	unsigned int bytes[8];
	unsigned char SPCbyte[1];

	fseek(f, 11, SEEK_SET);
	fread(bytes, sizeof(char), 2, f);
	bb.BPB_BytsPerSec = bytes[0];

	fseek(f, 13, SEEK_SET);
	fread(SPCbyte, sizeof(char), 1, f);
	bb.BPB_SecPerClus = SPCbyte[0];

	fseek(f, 14, SEEK_SET);
	fread(bytes, sizeof(char), 2, f);
	bb.BPB_RsvdSecCnt = bytes[0];

	fseek(f, 16, SEEK_SET);
	fread(bytes, sizeof(char), 1, f);
	bb.BPB_NumFATs = bytes[0];

	fseek(f, 32, SEEK_SET);
	fread(bytes, sizeof(char), 4, f);
	bb.BPB_TotSec32 = bytes[0];

	fseek(f, 36, SEEK_SET);
	fread(bytes, sizeof(char), 4, f);
	bb.BPB_FATSz32 = bytes[0];

	fseek(f, 44, SEEK_SET);
	fread(bytes, sizeof(char), 4, f);
	bb.BPB_RootClus = bytes[0];
}

void PrintBPB() {
	printf("Bytes per Sector: %u\n", bb.BPB_BytsPerSec);
	printf("Sectors per Cluster: %u\n", bb.BPB_SecPerClus);
	printf("Reserved Sector Count: %u\n", bb.BPB_RsvdSecCnt);
	printf("Number of FATs: %u\n", bb.BPB_NumFATs);
	printf("Total Sectors: %u\n", bb.BPB_TotSec32);
	printf("FAT Size: %u\n", bb.BPB_FATSz32);
	printf("Root Cluster: %u\n", bb.BPB_RootClus);
}
