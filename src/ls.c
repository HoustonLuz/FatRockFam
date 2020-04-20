#include "ls.h"
#include "BPB.h"

void ls(const char* DIRNAME, FILE* f, unsigned int clust) {
	struct DIRENTRY dir;
	unsigned int	offset,
			index;
	char		buf[32];
	
		if(strcmp(DIRNAME, ".") == 0) {
			PrintFileContents(clust, f);
		} else if (strcmp(DIRNAME, "..") == 0) {
			PrintFileContents(clust, f);
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

						if (!strcmp(dir.DIR_Name, DIRNAME))
							printf("%s\n", dir.DIR_Name);
					}

					index += 32;
				} while (buf[0] != 0x00 && index < getBytsPerSec() * getSecPerClus());
				clust = NextCluster(clust, f);
			} while (clust < 0x0FFFFFF8);

//			PrintFileContents(clust, f);
//			printf("Directory does not exist.\n");
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

void format(DIRENTRY* dir, char *buf){
	int i;

	for(i = 0;i < 10;i++) {
		dir->DIR_Name[i] = buf[i];
		if(dir->DIR_Name[i] == ' ')
			dir->DIR_Name[i] = '\0';
	}
	dir->DIR_Name[10] = '\0';

}

/*
typedef struct DIRENTRY {
	unsigned char	DIR_Name[11],
			DIR_Attributes,
			DIR_FstClus[4]
			DIR_FileSize[4];
} DIRENTRY;
*/
