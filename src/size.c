#include "size.h"

void size (FILE* f, unsigned int clus, char* arg) {
	struct DIRENTRY dir;
	unsigned int	offset,
			index;
	char		buf[32];
	int		foundFlag = 0;

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

				if (!strcmp(dir.DIR_Name, arg)) {
					if (dir.DIR_Attributes == 0x10)
						foundFlag = 2;
					else {
						printf("%s size is %d.\n", dir.DIR_Name, dir.DIR_FileSize);
						foundFlag=1;
					}
				}
			}

			index += 32;
		} while (buf[0] != 0x00 && index < 512);
		clus = NextCluster(clus, f);
	} while (clus < 0x0FFFFFF8);

	if (foundFlag == 0)
		printf("File not found.\n");
	else if (foundFlag == 2)
		printf("File is a directory.\n");
}
