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
					printf("%s size is %d.\n", dir.DIR_Name, dir.DIR_FileSize);
					foundFlag++;
				}
			}

			index += 32;
		} while (buf[0] != 0x00 && index < getBytsPerSec() * getSecPerClus());
		clus = NextCluster(clus, f);
	} while (clus < 0x0FFFFFF8);

	if (!foundFlag)
		printf("File not found.\n");
}
