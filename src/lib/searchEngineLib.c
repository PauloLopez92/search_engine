#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_LINE_LENGTH 256


// To get a whole line from file
char* getLine(const char* filename, int lineNumber) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    int currentLine = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        currentLine++;
        if (currentLine == lineNumber) {
            fclose(file);
            return strdup(line);
        }
    }

    fclose(file);
    return NULL;
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
