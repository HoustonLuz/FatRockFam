#include "open.h"

flagList	list;

int permissions(char* name, unsigned int clus) {
	int i = locate(name, clus);

	if(i < 0)
		return -1;
	else
		return list.flags[i];
}

void myOpen (FILE* f, unsigned int clus, char* fName, int flag) {
	DIRENTRY	dir;
	unsigned int	offset,
			index,
			i;
	char		buf[32];

	/*
	if(flag == 3)
		for(i = 0;i < list.size;i++)
			printf("%s %08X %d\n", list.names[i], list.clus[i], list.flags[i]);
	*/

	do {
		offset = LocateDir(clus);
		index = 0;

		do {
			fseek(f, offset + index, SEEK_SET);
			fread(buf, sizeof(char), 32, f);

			// entry not allocated/end of dir
			if (buf[0] == 0x00 || buf[0] == 0xE5) {
				continue;
			}

			if (buf[11] != 0x0F) {
				format(&dir, buf);

				if (strcmp(dir.DIR_Name, fName) == 0
				&& dir.DIR_Attributes != 0x10) {
					listAdd(fName, clus, flag);
					return;
				}
			}

			index += 32;
		} while (buf[0] != 0x00 && index <= 512);
		clus = NextCluster(clus, f);
	} while (clus < 0x0FFFFFF8);

	printf("File not found\n");
}

void myClose (FILE* f, unsigned int clus, const char* fName) {
	DIRENTRY	dir;
	unsigned int	offset,
			index,
			i;
	char		buf[32];

	do {
		offset = LocateDir(clus);
		index = 0;

		do {
			fseek(f, offset + index, SEEK_SET);
			fread(buf, sizeof(char), 32, f);

			// entry not allocated/end of dir
			if (buf[0] == 0x00 || buf[0] == 0xE5) {
				continue;
			}

			if (buf[11] != 0x0F) {
				format(&dir, buf);

				if (strcmp(dir.DIR_Name, fName) == 0
				&& dir.DIR_Attributes != 0x10) {
					listRm(fName, clus);
					return;
				}
			}

			index += 32;
		} while (buf[0] != 0x00 && index <= 512);
		clus = NextCluster(clus, f);
	} while (clus < 0x0FFFFFF8);

	printf("File not found\n");
}

void listRm (const char* name, unsigned int clus) {
	int 	i = locate(name, clus);

	if (i != -1) {
		for (;i < list.size - 1;i++) {
			strcpy(list.names[i], list.names[i + 1]);
			list.clus[i] = list.clus[i + 1];
			list.flags[i] = list.flags[i + 1];
		}

		list.size--;
	} else {
		printf("File not opened\n");
	}
}

void listAdd (char* name, unsigned int clus, int flags) {
	if(list.size + 1 == list.cap)
		allocMoreOpen();

	if (locate(name, clus) == -1) {
		strcpy(list.names[list.size], name);
		list.clus[list.size] = clus;
		list.flags[list.size] = flags;

		list.size++;
	} else {
		printf("File already opened\n");
	}
}

int locate (const char* name, unsigned int clus) {
	int i;

	for(i = 0;i < list.size;i++)
		if(clus == list.clus[i]
		&& !strcmp(list.names[i], name))
			return i;

	return -1;
}

void allocMoreOpen() {
	int		i;
	flagList	newList;

	newList.names = (char**) malloc(sizeof(char*) * (10 + list.cap));
	newList.clus = (unsigned int*) malloc(sizeof(unsigned int) * (10 + list.cap));
	newList.flags = (int*) malloc(sizeof(int) * (10 + list.cap));

	for (i = 0;i < list.cap + 10;i++) {
		if (i < list.size) {
			newList.names[i] = list.names[i];
			list.names[i] = 0;
			newList.clus[i] = list.clus[i];
			newList.flags[i] = list.flags[i];
		} else {
			newList.names[i] = (char*) malloc(sizeof(char) * 17);
		}
	}

	free(list.names);
	list.names = newList.names;
	newList.names = 0;

	free(list.clus);
	list.clus = newList.clus;
	newList.clus = 0;

	free(list.flags);
	list.flags = newList.flags;
	newList.flags = 0;

	list.cap += 10;
}

void listInit() {
	int i;

	list.names = (char**) malloc(sizeof(char*) * 10);
	for(i = 0;i < 10;i++)
		list.names[i] = (char*) malloc(sizeof(char) * 17);

	list.clus = (unsigned int*) malloc(sizeof(unsigned int) * 10);
	list.flags = (int*) malloc(sizeof(int) * 10);
	list.cap = 10;
	list.size = 0;
}

void listDest() {
	int i;

	for(i = 0;i < list.cap;i++)
		free(list.names[i]);
	free(list.names);

	free(list.clus);
	free(list.flags);
}
