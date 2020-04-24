#include "creat.h"

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
	int		i,
			hi, lo;

	hi = dir->DIR_FstClus << 16;
	lo = dir->DIR_FstClus % 0x10000;


	fseek(f, offset, SEEK_SET);
	for (i = 0;i < 11;i++) {
		if(i < strlen(dir->DIR_Name))
			fputc(dir->DIR_Name[i], f);
		else
			fputc((unsigned char) 0x20, f);
	}

	fputc((unsigned char) 0x20, f);
	for(i = 0;i < 8;i++)
		fputc((unsigned char) 0xFF, f);

	fputc((unsigned char) (hi % 0x100), f);
	fputc((unsigned char) (hi>>2), f);

	for(i = 0;i < 4;i++)
		fputc((unsigned char) 0xFF, f);

	fputc((unsigned char) (lo % 0x100), f);
	fputc((unsigned char) (lo>>2), f);

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

	return index;
}
