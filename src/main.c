#include "main.h"

char	**userInstr;
int	userArgs;
unsigned int CLUSTER = 0x02;

int main(int argc, const char* argv[]){
	int	exitFlag = 0,
		i;

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
			if (strcmp(userInstr[0],"exit") == 0//) {
			//REMOVE THIS BEFORE TURNIN
			|| !strcmp(userInstr[0],"x")){
				exitFlag = 1;
			} else if (strcmp(userInstr[0],"clus") == 0) {
				//REMOVE BEFORE TURNIN
				printf("%d is the cluster.\n", CLUSTER);
			} else if (strcmp(userInstr[0],"size") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: size \"FILENAME\"\n");
				else
					size(img, CLUSTER, userInstr[1]);
			} else if (strcmp(userInstr[0],"creat") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: creat \"FILENAME\"\n");
				else
					creat(img, CLUSTER, userInstr[1]);

			} else if (strcmp(userInstr[0],"cd") == 0) {
				if(userArgs == 1 || userArgs > 2)
					printf("usage: cd \"DIRECTORY\"\n");
				else
					CLUSTER = cd(img, CLUSTER, userInstr[1]);
			} else if (strcmp(userInstr[0],"info") == 0) {
				PrintBPB();
			} else if (strcmp(userInstr[0],"ls") == 0) {
				if(userArgs == 1)
					ls(".", img, CLUSTER);
				else
					ls(userInstr[1], img, CLUSTER);
			} else {
				//else should not catch any specific instruction.
				//As such leave this code in so that you know when
				// an error occurs in catching any instruction.
				printf("Command WIP.\n");
			}
		} else {
			//Blank instruction, print error.
			printf("Please enter an instruction.\n");
		}

	} while (exitFlag == 0);

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
