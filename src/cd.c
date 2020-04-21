#include "cd.h"

unsigned int cd (FILE* f, unsigned int clus, char* arg) {
	struct DIRENTRY dir;
	unsigned int	offset,
			index,
			stClus = clus;
	char		buf[32];
	int		foundFlag = 0;

	if(strcmp(arg, ".") == 0)
		return clus;
	if(!strcmp(arg, "..") && clus == 2){
		printf("Directory not found.\n");
		return clus;
	}

	do {
		offset = LocateDir(clus);
		index = 0;

		do {
			fseek(f, offset + index, SEEK_SET);
			fread(buf, sizeof(char), 32, f);

			// entry not allocated/end of dir
			if (buf[0] == 0x00 || buf[0] == 0xE5)
				continue;
			if (buf[11] != 0x0F) {

				format(&dir, buf);

				if (strcmp(dir.DIR_Name, arg) == 0
				&& dir.DIR_Attributes == 0x10) {
					if(dir.DIR_FstClus < 2)
						return 2;
					else if (dir.DIR_FstClus < 268435448) {
						return dir.DIR_FstClus;
					}
				} else
					foundFlag = 2;
			}

			index += 32;
		} while (buf[0] != 0x00 && index < 512);
		clus = NextCluster(clus, f);
	} while (clus < 0x0FFFFFF8);

	if (foundFlag == 0)
		printf("Directory not found.\n");
	else if (foundFlag == 2)
		printf("Cannot change directory to non-directory.\n");


	return stClus;
}
