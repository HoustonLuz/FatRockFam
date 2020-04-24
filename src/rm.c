#include "rm.h"

void rm(const char* FILENAME, FILE* f, unsigned int clust) {
	struct DIRENTRY dir, fromDir;
	unsigned int offset, index;
	unsigned int fromIndex, fromClust;
	char buf[32];
	char bufCopy[32];
	int 	fromFlag = 0,	// 0: DNE, 1: DIR, 2: FILE
			i = 0,
			lastEntry = 0;

	// loop to validate FROM and TO entries
	do {
		offset = LocateDir(clust);
		index = 0;
		do {
			fseek(f, offset + index, SEEK_SET);
			fread(buf, sizeof(char), 32, f);

			if (buf[0] == 0x00 || buf[0] == 0xE5)
				continue;
			if (buf[11] != 0x0F) {
				format(&dir, buf);

				// check if FILENAME exists
				if (strcmp(dir.DIR_Name, FILENAME) == 0) {
					// check if FILENAME is directory
					if (dir.DIR_Attributes == 0x10)
						fromFlag = 1;
					else	// FILENAME is a file
						fromFlag = 2;

					if(dir.DIR_FstClus < 2)
						dir.DIR_FstClus = 2;

					for(i=0;i<32;i++)
						bufCopy[i] = buf[i];

					fromIndex = index;
					fromClust = clust;

					if(index + 32 == 512)	// last entry
						lastEntry = 1;
				}
			}
			index += 32;
		} while (buf[0] != 0x00 && index < 512);
		clust = NextCluster(clust, f);
	} while (clust < 0x0FFFFFF8);
	// end loop validation

	// if FILENAME is a file, remove file
	if (fromFlag == 2) {
		// set old DIRENTRY as empty
		for(i=0;i<32;i++) {
			bufCopy[i] = 0x0;
		}
		offset = LocateDir(fromClust);
		fseek(f, offset + fromIndex, SEEK_SET);
		if(lastEntry == 1)
			fwrite(&bufCopy, 32, 1, f);
		else {
			bufCopy[0] = 0xE5;
			fwrite(&bufCopy, 32, 1, f);
		}
	}
	else if (fromFlag == 1) {
		printf("rm: cannot remove %s: Is a directory\n", FILENAME);
	}
	else {
		printf("rm: cannot remove %s: No such file or directory\n", FILENAME);
	}
}

