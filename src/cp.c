#include "cp.h"

void cp(const char* FILENAME, const char* TO, FILE* f, unsigned int clust) {
	struct DIRENTRY dir;
	struct DIRENTRY toDir, fromDir;
	unsigned int offset, index, toFstClus;
	unsigned int fromIndex, fromClust;
	unsigned int toIndex, toClust;
	char buf[32];
	char bufCopy[32];
	char FstClusHI[2], FstClusLO[2], FileSize[4];
	int 	fromFlag = 0,	// 0: DNE, 1: DIR, 2: FILE
			toFlag = 0, 	// 0: DNE, 1: DIR, 2: FILE
			i = 0,
			lastEntry = 0;

	// loop to validate FILENAME and TO entries
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

				// check if TO exists
				if (strcmp(dir.DIR_Name, TO) == 0) {
					// check if TO is directory
					if (dir.DIR_Attributes == 0x10)
						toFlag = 1;
					else	// TO is a file
						toFlag = 2;

					if(dir.DIR_FstClus < 2)
						dir.DIR_FstClus = 2;

					toIndex = index;
					toClust = clust;
					toFstClus = dir.DIR_FstClus;

					if(index + 32 == 512)	// last entry
						lastEntry = 1;
				}

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
				}
			}
			index += 32;
		} while (buf[0] != 0x00 && index < 512);
		clust = NextCluster(clust, f);
	} while (clust < 0x0FFFFFF8);
	// end loop validation

	// if TO is a directory and FILENAME is a file, move FILENAME entry to TO directory
	if (toFlag == 1 && fromFlag == 2) {
		offset = LocateDir(toFstClus);
		toIndex = 0;
		do {
			fseek(f, offset + toIndex, SEEK_SET);
			fread(buf, sizeof(char), 32, f);

			if (buf[0] == 0x00 || buf[0] == 0xE5) {
				fseek(f, offset + toIndex, SEEK_SET);
				fwrite(&bufCopy, 32, 1, f);

				break;
			}

			toIndex += 32;
		} while (buf[0] != 0x00 && index < 512);
	}
	// if TO does not exist, rename FILENAME entry to name specified by TO
	else if (toFlag == 0) {
		offset = LocateDir(fromClust);
		fseek(f, offset + fromIndex, SEEK_SET);

		for(i=0;i<10;i++) {
			if(TO[i] == ' ')
				fputc('\0', f);
			else
				fputc(TO[i], f);
		}
		fputc('\0', f);
	}
	// if TO is a file and FILENAME is a file, print error
	if (toFlag == 2 && fromFlag == 2) {
		printf("The name is already being used by another file\n");
	}
	// FILENAME is a directory, print error
	if (fromFlag == 1) {
		printf("Error: file is a directory\n");
	}
	// FILENAME does not exist
	else if (fromFlag == 0) {
		printf("Error: file does not exist\n");
	}
}

