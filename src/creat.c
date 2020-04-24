#include "creat.h"

void mkdir(FILE* f, unsigned int clus, char* DIRNAME) {
	struct DIRENTRY dir;
	unsigned int	offset,
			index,
			avail = 0,
			nextFree,
			stClus = clus;
	char		buf[32];

	if(strlen(DIRNAME) > 11) {
		printf("Filename too long.\n");
		return;
	}
				
	do {
		offset = LocateDir(clus);
		index = 0;

		do {
			fseek(f, offset + index, SEEK_SET);
			fread(buf, sizeof(char), 32, f);

			// entry not allocated/end of dir
			if (buf[0] == 0x00 || buf[0] == 0xE5) {
				avail = offset + index;
				continue;
			}

			if (buf[11] != 0x0F) {
				format(&dir, buf);

				if (strcmp(dir.DIR_Name, DIRNAME) == 0) {
					printf("%s\n", 
				"File or Directory with that name already exists.");
					return;
				}
			}

			index += 32;
		} while (buf[0] != 0x00 && index <= 512);
		clus = NextCluster(clus, f);
	} while (clus < 0x0FFFFFF8);

	dir.DIR_Name[0] = '\0';
	strcpy(dir.DIR_Name, DIRNAME);
	dir.DIR_Attributes = 0x10;
	dir.DIR_FileSize = 0;
	nextFree = findFirstFreeCluster(f);
	dir.DIR_FstClus = nextFree;
	addDirEnt(f, avail, &dir);

	//printf("%d to %X\n", nextFree, LocateDir(nextFree));

	dir.DIR_Name[0] = '\0';
	strcpy(dir.DIR_Name, ".");
	addDirEnt(f, LocateDir(nextFree), &dir);

	dir.DIR_Name[0] = '\0';
	strcpy(dir.DIR_Name, "..");
	dir.DIR_FstClus = stClus;
	addDirEnt(f, LocateDir(nextFree) + 0x20, &dir);
}

void creat (FILE* f, unsigned int clus, char* FILENAME) {
	struct DIRENTRY dir;
	unsigned int	offset,
			index,
			avail = 0,
			finalFlag = 0;
	char		buf[32];

	if(strlen(FILENAME) > 11) {
		printf("Filename too long.\n");
		return;
	}
				
	do {
		offset = LocateDir(clus);
		index = 0;

		do {
			fseek(f, offset + index, SEEK_SET);
			fread(buf, sizeof(char), 32, f);

			// entry not allocated/end of dir
			if (buf[0] == 0x00 || buf[0] == 0xE5) {
				avail = offset + index;
				continue;
			}

			if (buf[11] != 0x0F) {
				format(&dir, buf);

				if (strcmp(dir.DIR_Name, FILENAME) == 0) {
					printf("%s\n", 
				"File or Directory with that name already exists.");
					return;
				}
			}

			index += 32;
		} while (buf[0] != 0x00 && index <= 512);
		clus = NextCluster(clus, f);
	} while (clus < 0x0FFFFFF8);

	dir.DIR_Name[0] = '\0';

	strcpy(dir.DIR_Name, FILENAME);
	dir.DIR_Attributes = 0x20;
	dir.DIR_FileSize = 0;
	dir.DIR_FstClus = findFirstFreeCluster(f);
	addDirEnt(f, avail, &dir);
}

void addDirEnt (FILE* f, unsigned int offset, DIRENTRY* dir) {
	unsigned char	buf[4];
	unsigned int	i,
			hi, lo,
			tmp;

	//printf("%X offset.\n", offset);

	lo = dir->DIR_FstClus % 0x10000; 
	hi = (dir->DIR_FstClus - lo) >> 4;

	/*
	printf("%08X\n%04X %04X\n", dir->DIR_FstClus, hi, lo);
	printf("%s %02X\n", "hi up", (hi >> 2));
	printf("%s %02X\n", "hi down", (hi % 0x100));
	printf("%s %02X\n", "lo up", (lo >> 2));
	printf("%s %02X\n", "hi down", (lo % 0x100));
	*/

	fseek(f, offset, SEEK_SET);
	for (i = 0;i < 11;i++) {
		if(i < strlen(dir->DIR_Name))
			fputc(dir->DIR_Name[i], f);
		else
			fputc((unsigned char) 0x20, f);
	}

	fputc(dir->DIR_Attributes, f);

	fputc((unsigned char) 0x00, f);
	for(i = 0;i < 7;i++)
		fputc((unsigned char) 0xFF, f);

	tmp = hi % 0x100;
	fputc((unsigned char) tmp, f);
	tmp = (hi - tmp) >> 8;
	fputc((unsigned char) tmp, f);

	for(i = 0;i < 4;i++)
		fputc((unsigned char) 0xFF, f);

	tmp = lo % 0x100;
	fputc((unsigned char) tmp, f);
	tmp = (lo - tmp) >> 8;
	fputc((unsigned char) tmp, f);

	for(i = 0;i < 4;i++)
		fputc((unsigned char) 0x00, f);

}

unsigned int findFirstFreeCluster(FILE* f) {
	unsigned char	buf[4];
	unsigned int	FatSt,
			FatEn,
			clusV,
			i,
			index = 0;

	FatSt = getRootClustNum() * 4;
	FatSt = (getRsvdSecCnt() + (FatSt / getBytsPerSec())) * getBytsPerSec();
	FatEn = getFatSize() * getBytsPerSec() + FatSt;

	do {
		fseek(f, FatSt + (index * 4), SEEK_SET);
		fread(buf, sizeof(char), 4, f);
		clusV = 0;

		for(i = 0;i < 4;i++)
			clusV += buf[i];

		index++;
	} while (clusV != 0 && (FatSt + index) <= FatEn);

	fseek(f, FatSt + (index * 4) - 4, SEEK_SET);

	fputc((unsigned char) 0xFF, f);
	fputc((unsigned char) 0xFF, f);
	fputc((unsigned char) 0xFF, f);
	fputc((unsigned char) 0xF8, f);

	return index;
}
