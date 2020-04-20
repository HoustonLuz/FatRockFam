#include "ls.h"
#include "BPB.h"

void ls(const char* DIRNAME, FILE* f, unsigned int clust) {
	int i = 0;
		if(strcmp(DIRNAME, ".") == 0) {
			PrintFileContents(clust, f);
		} else if (strcmp(DIRNAME, "..") == 0) {
			PrintFileContents(clust, f);
		} else {
			PrintFileContents(clust, f);
			printf("Directory does not exist.\n");
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

unsigned int FindCluster(unsigned int currentClust, FILE* f, char* path) {
	unsigned int clust;
	fseek(f, LocateDir(currentClust), SEEK_SET);
	fread(&clust, sizeof(unsigned int), 1, f);
	
	// function to return each dir one by one until match
	if(strcmp(path, "") == 0)
		return clust;
	else
		FindCluster(NextCluster(currentClust, f), f, path);
}
