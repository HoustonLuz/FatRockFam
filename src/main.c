#include "main.h"

char	**userInstr;
int	userArgs;
unsigned int CLUSTER = 0x02;

int main(int argc, const char* argv[]){
	int	exitFlag = 0,
		i;

	listInit();

	//Open file
	FILE* img = fopen(argv[1], "r+");
	if(img == 0){
		printf("Incorrect image file.\n");
		return -1;
	}
	else {
		ReadBPB(img);
	}

	do {
		getUserIn();		

		if (userArgs > 0) {
			//Normal Execution
			if (strcmp(userInstr[0],"exit") == 0) {
				exitFlag = 1;
			} else if (strcmp(userInstr[0],"size") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: size \"FILENAME\"\n");
				else
					size(img, CLUSTER, userInstr[1]);
			} else if (strcmp(userInstr[0],"open") == 0) {
				if(userArgs != 3)
					printf("usage: open \"FILENAME\" MODE \n");
				else {
					if(!strcmp(userInstr[2], "r"))
						myOpen(img, CLUSTER, userInstr[1], 1);
					else if(!strcmp(userInstr[2], "w"))
						myOpen(img, CLUSTER, userInstr[1], 2);
					else if(!strcmp(userInstr[2], "rw")
					|| !strcmp(userInstr[2], "wr"))
						myOpen(img, CLUSTER, userInstr[1], 3);
					else
						printf("valid flags are: r w rw wr\n");
				}
			} else if (strcmp(userInstr[0],"close") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: close \"FILENAME\"\n");
				else
					myClose(img, CLUSTER, userInstr[1]);
			} else if (strcmp(userInstr[0],"read") == 0) {
				if(userArgs != 4)
					printf("usage: read \"FILENAME\" \"OFFSET\" \"SIZE\"\n");
				else
					read(userInstr[1], userInstr[2], userInstr[3], img, CLUSTER);
			} else if (strcmp(userInstr[0],"write") == 0) {
				if(userArgs != 5)
					printf("usage: write FILENAME OFFSET SIZE STRING\n");
				else
					write(userInstr[1], userInstr[2], userInstr[3], userInstr[4], img, CLUSTER);
			} else if (strcmp(userInstr[0],"creat") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: creat \"FILENAME\"\n");
				else
					creat(img, CLUSTER, userInstr[1]);
			} else if (strcmp(userInstr[0],"mkdir") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: mkdir \"DIRNAME\"\n");
				else
					mkdir(img, CLUSTER, userInstr[1]);
			} else if (strcmp(userInstr[0],"cd") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: cd \"DIRECTORY\"\n");
				else
					CLUSTER = cd(img, CLUSTER, userInstr[1]);
			} else if (strcmp(userInstr[0],"mv") == 0) {
				if(userArgs != 3)
					printf("usage: mv \"FROM\" \"TO\"\n");
				else
					mv(userInstr[1], userInstr[2], img, CLUSTER);
			} else if (strcmp(userInstr[0],"cp") == 0) {
				if(userArgs == 4)
					if(userInstr[1] == "-r")
						printf("Extra credit not implemented yet\n");
					else
						printf("usage: cd -r \"FROM\" \"TO\"\n");
				else if (userArgs == 3)
					cp(userInstr[1], userInstr[2], img, CLUSTER);
				else
					printf("usage: cd \"FILENAME\" \"TO\"\n");
			} else if (strcmp(userInstr[0],"rm") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: rm \"FILENAME\"\n");
				else
					rm(userInstr[1], img, CLUSTER, 0);
			} else if (strcmp(userInstr[0],"rmdir") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: rmdir DIRNAME\n");
				else
					rm(userInstr[1], img, CLUSTER, 1);
			} else if (strcmp(userInstr[0],"info") == 0) {
				PrintBPB();
			} else if (strcmp(userInstr[0],"ls") == 0) {
				if(userArgs == 1)
					ls(".", img, CLUSTER);
				else
					ls(userInstr[1], img, CLUSTER);
			} else {
				printf("Unknown command\n");
			}
		} else {
			//Blank instruction, print error.
			printf("Please enter an instruction.\n");
		}

	} while (exitFlag == 0);

	listDest();

	//Deallocate user arguments
	for(i = 0;i < 5;i++)
		free(userInstr[i]);
	free(userInstr);

	//printf("Project 3 successfully executed.\n");

	return 0;
}

void getUserIn(){
	char	peek;
	int 	i;

	//Check if userInstr is already allocated.
	if(userInstr == 0){
		//Allocate max size.
		userInstr = (char**) malloc(sizeof(char*) * 5);
		for (i = 0;i < 5;i++) {
			userInstr[i] = (char*) malloc(sizeof(char) * 17);
			userInstr[i][0] = '\0';
		}

		//printf("Allocated user instruction.\n");
	}

	//Print prompt.
	printf("$ ");

	//Read successive strings until 5 arguments are read or
	// until a new line.
	i = 0;
	peek = getc(stdin);
	while (peek != '\n' && i != 5) {
		if(peek != ' '){
			ungetc(peek, stdin);
			scanf("%16s", userInstr[i]);
			i++;
		}

		peek = getc(stdin);
	}

	userArgs = i;

	/*
 	//print parsed command
	for (i = 0;i < userArgs;i++)
		printf("%s ", userInstr[i]);
	if (i != 0)
		printf("\n");
	*/
}
