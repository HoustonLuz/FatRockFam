#include "ls.h"
#include "BPB.h"

void ls(const char* DIRNAME, FILE* f, unsigned int clust) {
	struct DIRENTRY dir;
	unsigned int	offset,
			index;
	int err = 1;
	char		buf[32];
	
		if(strcmp(DIRNAME, ".") == 0) {
			PrintFileContents(clust, f);
		} else if (!strcmp(DIRNAME, "..") && clust == 2) {
					printf("Directory not found.\n");
		} else {

			do {
				offset = LocateDir(clust);
				index = 0;

				do {
					fseek(f, offset + index, SEEK_SET);
					fread(buf, sizeof(char), 32, f);

					// entry not allocated/end of dir
					if (buf[0] == 0x00 || buf[0] == 0xE5)
						continue;
					if (buf[11] != 0x0F) {

						format(&dir, buf);

						if (strcmp(dir.DIR_Name, DIRNAME) == 0
						&& dir.DIR_Attributes == 0x10) {
							if(dir.DIR_FstClus < 2)
								dir.DIR_FstClus = 2;
							PrintFileContents(dir.DIR_FstClus, f);
							err = 0;
							return;
						}
					}

					index += 32;
				} while (buf[0] != 0x00 && index < 512);
				clust = NextCluster(clust, f);
			} while (clust < 0x0FFFFFF8);

			if(err == 1)
				printf("Directory not found.\n");
		}
}

void PrintFileContents(int clustNum, FILE* f) {
	struct DIRENTRY dir;
	char FileName[9];
	char FileExt[4];
	int offset, i;
	int index = 0;

	do {
		offset = LocateDir(clustNum);

		do {
			fseek(f, offset, SEEK_SET);
			fread(&dir, sizeof(struct DIRENTRY), 1, f);
			offset += 32;

			if(dir.DIR_Name[0] == 0x00 || dir.DIR_Name[0] == 0xE5)	// entry not allocated/end of dir
				continue;

			// if attribute is set to directory or archive value
			if(dir.DIR_Attributes == 0x10 || dir.DIR_Attributes == 0x20) {
				for(i = 0; i < 8; i++) {
					if(dir.DIR_Name[i] != ' ')
						FileName[i] = dir.DIR_Name[i];
					else {
						FileName[i] = '\0';
						break;
					}
				}
				FileName[8] = '\0';

				for(i = 0; i < 3; i++) {
					if(dir.DIR_Name[i+8] != ' ')
						FileExt[i] = dir.DIR_Name[i+8];
					else {
						FileExt[i] = '\0';
						break;
					}
				}
				FileExt[3] = '\0';

				if(strcmp(FileName, ".") != 0 && strcmp(FileName, "..") != 0) {
					printf("%s\n", FileName);
					if(FileExt[0] != '\0')	// file has extension
						printf("%s.%s\n", FileName, FileExt);
				}
			}
		} while (offset < LocateDir(clustNum) + getBytsPerSec());
		clustNum = NextCluster(clustNum, f);
	} while (clustNum < 0x0FFFFFF8);
}

unsigned int NextCluster(unsigned int clust, FILE* f) {
	unsigned int next;
	long offset = getRsvdSecCnt() * getBytsPerSec() + clust * 4;
	fseek(f, offset, SEEK_SET);
	fread(&next, sizeof(unsigned int), 1, f);
	return next;
}

void format(DIRENTRY* dir, unsigned char *buf){
	int i;

	for(i = 0;i < 10;i++) {
		dir->DIR_Name[i] = buf[i];
		if(dir->DIR_Name[i] == ' ')
			dir->DIR_Name[i] = '\0';
	}
	dir->DIR_Name[10] = '\0';

	dir->DIR_Attributes = buf[11];

	dir->DIR_FstClus = 0;
	dir->DIR_FstClus += (unsigned int)(buf[21])<<(24);
	dir->DIR_FstClus += (unsigned int)(buf[20])<<(16);
	dir->DIR_FstClus += (unsigned int)(buf[27])<<(8);
	dir->DIR_FstClus += (unsigned int) buf[26];

	dir->DIR_FileSize = 0;
	for(i = 0; i < 4; i++)
		dir->DIR_FileSize += (unsigned int) buf[i+28];
/*
	printf("DIR_Name: %s\n", dir->DIR_Name);
	printf("DIR_Attributes: %X\n", dir->DIR_Attributes);
	printf("DIR_FstClus: %08X\n", dir->DIR_FstClus);
	printf("DIR_FileSize: %X\n", dir->DIR_FileSize);
*/
}

