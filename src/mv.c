#include "mv.h"

void mv(const char* FROM, const char* TO, FILE* f, unsigned int clust) {
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

				// check if FROM exists
				if (strcmp(dir.DIR_Name, FROM) == 0) {
					// check if FROM is directory
					if (dir.DIR_Attributes == 0x10)
						fromFlag = 1;
					else	// FROM is a file
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

	if (fromFlag == 0) {
		printf("Cannot move directory: invalid argument\n");
		return;
	}

	// if TO is a directory, move FROM entry to be inside the TO directory
	if (toFlag == 1) {
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
	// if TO does not exist, rename FROM entry to name specified by TO
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
	// if TO is a file and FROM is a file, print error
	else if (fromFlag == 2) {
		printf("The name is already being used by another file\n");
	}
	// if TO is a file and FROM is a directory, print error
	else if (fromFlag == 1) {
		printf("Cannot move directory: invalid destination argument\n");
	}
	else {
		printf("Cannot move directory: invalid argument\n");
	}
}

