#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_LINE_LENGTH 256

// To print Help
void printHelp() {
	printf("Usage: program [-f filename] [ -c <ID number>]\n");
	printf("\t\tor\n");
	printf("Usage: program [-f filename] [ -n '<name>', -d DD/MM/YYYY ]\n");
	printf("\n");
	printf("Options:\n");
	printf("  -f		 Specify a filename\n");
	printf("  -i             Specify a search for ID	searchEngine -f <filename> -i <ID Number>\n");
	printf("  -n             Specify a search for Name	searchEngine -f <filename> -n '<name>'\n");
	printf("  -d             Specify a search for Birthday	searchEngine -f <filename> -d DD/MM/YYYY\n");
	printf("  -d             Specify a search for Day/Month	searchEngine -f <filename> -d DD/MM\n");
	printf("  -d             Specify a search for Year	searchEngine -f <filename> -d YYYY\n");
}

// To Print a whole line from file
void printLine(const char* filename, int lineNumber) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open the file.\n");
		return;
	}
	char line[MAX_LINE_LENGTH];
	int currentLine = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
		currentLine++;
		if (currentLine == lineNumber) {
			//printf("Line %d:\n%s", lineNumber, line);
			printf(line);
			break;
		}
	}
	fclose(file);
}
// To check if all words from a search are into a sentence
int containsAllWords(const char* searchWords, const char* sentence) {
	char sentenceCopy[1000];
	strcpy(sentenceCopy, sentence);

	char* token = strtok(sentenceCopy, " ");
	while (token != NULL) {
		if (strstr(searchWords, token) == NULL) {
		return 0; // At least one word is not present
		}
		token = strtok(NULL, " ");
	}
	return 1; // All words are present
}
// To search for an id
int searchID(const char* filename, const char* searchTerm) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open the file.\n");
		return 0;
	}
	char line[MAX_LINE_LENGTH];
	int lineNum = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
		lineNum++;
		char name[MAX_LINE_LENGTH];
		if (sscanf(line, "%[^;]", name) == 1) {
			if (strcmp(name, searchTerm) == 0) {
				fclose(file);
				return lineNum;
			}
		}
	}
	fclose(file);
	return 0;
}
// To search for an incomplete or complete name
int searchHalfNameAll(const char* filename, const char* searchTerm, int startLine) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open the file.\n");
		return -1;
	}

	char line[MAX_LINE_LENGTH];
	int lineNum = 0;

	// Skip lines until the startLine
	while (fgets(line, sizeof(line), file) != NULL) {
		lineNum++;
		if (lineNum >= startLine) {
			break;
		}
	}

	// Search for the searchTerm from the startLine onwards
	while (fgets(line, sizeof(line), file) != NULL) {
		lineNum++;
		char name[MAX_LINE_LENGTH];
		if (sscanf(line, "%*[^;];%[^;]", name) == 1) {
			if (containsAllWords(name, searchTerm) == 1) {
				fclose(file);
				return lineNum;
			}
		}
	}
	fclose(file);
	return -1;
}

// To search for a date 
int searchDateAll(const char* filename, const char* date, int startLine) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open the file.\n");
		return -1;
	}

	char line[MAX_LINE_LENGTH];
	int dateLength = strlen(date);
	int lineNum = 0;

	// Skip lines until the startLine
	while (fgets(line, sizeof(line), file) != NULL) {
		lineNum++;
		if (lineNum >= startLine) {
			break;
		}
	}
	// Search for the searchTerm from the startLine onwards
	if (dateLength==10){
		while (fgets(line, sizeof(line), file) != NULL) {
			lineNum++;
			char name[MAX_LINE_LENGTH];
			if (sscanf(line, "%*[^;];%*[^;];%*[^;];%10s", name) == 1) {
				//printf("Target:%s ",cleanDate(date));
				//printf("Read:%s\n",name);
				if (strcmp(name, date) == 0) {
					fclose(file);
					return lineNum;
				}
			}
		}
	}else if (dateLength==5){
		while (fgets(line, sizeof(line), file) != NULL) {
			lineNum++;
			char name[MAX_LINE_LENGTH];
			if (sscanf(line, "%*[^;];%*[^;];%*[^;];%5s", name) == 1) {
				//printf("Target:%s ",cleanDate(date));
				//printf("Read:%s\n",name);
				if (strcmp(name, date) == 0) {
					fclose(file);
					return lineNum;
				}
			}
		}
	}else if (dateLength==4){
		while (fgets(line, sizeof(line), file) != NULL) {
			lineNum++;
			char name[MAX_LINE_LENGTH];
			if (sscanf(line, "%*[^;];%*[^;];%*[^;];%10s", name) == 1) {
				//printf("Target:%s ",cleanDate(date));
				//printf("Read:%s\n",name);
				if (strstr(name, date) != NULL) {
					fclose(file);
					return lineNum;
				}
			}
		}
	}
	fclose(file);
	return -1;
}

// To search for an incomplete name and a date
int searchHalfNameDateAll(const char* filename, const char* iname, const char* date, int startLine) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open the file.\n");
		return -1;
	}

	char line[MAX_LINE_LENGTH];
	int lineNum = 0;
	int dateLength = strlen(date);

	// Skip lines until the startLine
	while (fgets(line, sizeof(line), file) != NULL) {
		lineNum++;
		if (lineNum >= startLine) {
			break;
		}
	}
	// Search for the searchTerm from the startLine onwards
	if (dateLength==10){
		while (fgets(line, sizeof(line), file) != NULL) {
			lineNum++;
			char name[MAX_LINE_LENGTH];
			if (sscanf(line, "%*[^;];%[^;]", name) == 1) {
				if (containsAllWords(name, iname)) {
					if (sscanf(line, "%*[^;];%*[^;];%*[^;];%10s", name) == 1) {
						if (strcmp(name, date) == 0) {
							fclose(file);
							return lineNum;
						}
					}
				}
			}
		}
	} else if (dateLength==5){
		while (fgets(line, sizeof(line), file) != NULL) {
			lineNum++;
			char name[MAX_LINE_LENGTH];
			if (sscanf(line, "%*[^;];%[^;]", name) == 1) {
				if (containsAllWords(name, iname)) {
					if (sscanf(line, "%*[^;];%*[^;];%*[^;];%5s", name) == 1) {
						if (strcmp(name, date) == 0) {
							fclose(file);
							return lineNum;
						}
					}
				}
			}
		}
	} else if (dateLength==4){
		while (fgets(line, sizeof(line), file) != NULL) {
			lineNum++;
			char name[MAX_LINE_LENGTH];
			if (sscanf(line, "%*[^;];%[^;]", name) == 1) {
				if (containsAllWords(name, iname)) {
					char* rn = NULL;
					if (sscanf(line, "%*[^;];%*[^;];%*[^;];%10s", name) == 1) {
						if (strstr(name, date) != NULL) {
							fclose(file);
							return lineNum;
						}
					}
				}
			}
		}
	}
	fclose(file);
	return -1;
}
// To handle With Segmentation fault
void segfaultHandler(int signal) {
	printHelp();
	exit(signal);
}
int main(int argc, char *argv[]) {
	// Install the signal handler for SIGSEGV
	signal(SIGSEGV, segfaultHandler);

	char *flag_f = "-f";
	char *flag_i = "-i";
	char *flag_n = "-n";
	char *flag_d = "-d";
	char *filename = NULL;

	char *id = NULL;
	char *date = NULL;
	char *name = NULL;

	int lineNumber;

	int hasFlag_d = 0;
	int hasFlag_n = 0;


	if (argc == 1) {
		printHelp();
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], flag_f) == 0) {
			if (i + 1 < argc) {
				filename = argv[i + 1];
				i++; 
			}
		}
	}

	// Check presence
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], flag_i) == 0) {
			id = argv[i + 1];
			printLine(filename, searchID(filename, id));
			return 0;
			
		} else if (strcmp(argv[i], flag_d) == 0) {
			date = argv[i + 1];
			hasFlag_d = 1;

		} else if (strcmp(argv[i], flag_n) == 0) {
			name = argv[i + 1];
			hasFlag_n = 1;
		}
	}

	// Check each combination
	if (hasFlag_n && hasFlag_d) {
		while(lineNumber != -1){
			lineNumber = searchHalfNameDateAll(filename, name, date, lineNumber);
			printLine(filename, lineNumber);
			//printf("%i\n",lineNumber);
		}
	} else if (hasFlag_d) {
		while(lineNumber != -1){
			lineNumber = searchDateAll(filename, date, lineNumber);
			printLine(filename, lineNumber);
			//printf("%i\n",lineNumber);
		}
	} else if (hasFlag_n) {
		while(lineNumber != -1){
			lineNumber = searchHalfNameAll(filename, name, lineNumber);
			printLine(filename, lineNumber);
			//printf("%i\n",lineNumber);
		}
	}
	if (filename == NULL) {
		printHelp();
		return 0;
	}

	return 0;
}
