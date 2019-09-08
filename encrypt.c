#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void printUsageMessage();

void encryptFile(FILE *sourceFilePointer, FILE *destinationFilePointer, int shiftAmount);

int *shiftCharacter(int *character, int shiftAmount);

int isNumber(char *arg);

int isFileValid(char *arg);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printUsageMessage();
        return 1; // Program used with wrong arguments.
    }

    if (argv[1] == NULL || argv[2] == NULL) {
        printUsageMessage();
        return 1; // Program used with wrong arguments.
    }

    if (!isFileValid(argv[1])) {
        printf("Source file is not a .txt file. \n");
        printUsageMessage();
        return 1; // Program used with wrong arguments.
    }

    if (!isNumber(argv[2])) {
        printf("Second argument is not a number or is negative. \n");
        printUsageMessage();
        return 1; // Program used with wrong arguments.
    }

    int shiftAmount = atoi(argv[2]);

    if (shiftAmount < 1 || shiftAmount > 25) {
        printf("Second argument is not a number within range 1 - 25 (inclusive). \n");
        printUsageMessage();
        return 1; // Program used with wrong arguments.
    }

    char *sourceFileName = argv[1];
    char *destinationFileName = malloc(sizeof(sourceFileName) + 5);
    strcpy(destinationFileName, sourceFileName);
    strcat(destinationFileName, ".enc");
    FILE *sourceFilePointer;
    FILE *destinationFilePointer;
    sourceFilePointer = fopen(sourceFileName, "rb");
    destinationFilePointer = fopen(destinationFileName, "wb");

    if (sourceFilePointer == NULL) {
        printf("Could not open the source file. \n");
        return 2; //File problem.
    }

    if (destinationFilePointer == NULL) {
        printf("Could not open the destination file.\n");
        return 2; //File problem.
    }

    encryptFile(sourceFilePointer, destinationFilePointer, shiftAmount);
    fclose(sourceFilePointer);
    fclose(destinationFilePointer);
    free(destinationFileName);
    return 0; // Return with no problems.

}
// Gets all the characters from the file, encrypts only the ones that are in the alphabet and puts them on the encrypted file.
void encryptFile(FILE *sourceFilePointer, FILE *destinationFilePointer, int shiftAmount) {
    int ch;
    while ((ch = getc(sourceFilePointer)) != EOF) {
        if (isalpha(ch)) {
            int shiftedCharacter = *shiftCharacter(&ch, shiftAmount);
            putc(shiftedCharacter, destinationFilePointer);
        } else {
            putc(ch, destinationFilePointer);
        }
    }
}
// Shifts a given characters the inputted number.
int *shiftCharacter(int *character, int shiftAmount) {
    int newLetter = (*character + shiftAmount);
    if (*character >= 65 && *character <= 90) { // Checks if the character is a lowercase letter.
        if (newLetter > 90) { // Makes sure new letter is within the lowercase character range.
            newLetter = (newLetter % 90) + 64; // (Wrap - around)
        }
    } else if (*character >= 97 && *character <= 122) { // Checks if the character is an uppercase letter.
        if (newLetter > 122) { // Makes sure new letter is within the uppercase character range.
            newLetter = (newLetter % 122) + 96; // (Wrap - around)
        }
    }
    *character = newLetter;
    return character;
}

void printUsageMessage() {
    printf("Usage : encrypt arg1 arg2 \n");
    printf("arg1 : the name of the file to be encrypted \n");
    printf("arg2 : number of shifts for each character \n");
    printf("e.g. encrypt message.txt 3 \n");
}
// Checks if a given string is a number.
int isNumber(char *arg) {
    int i;
    char numberArg[strlen(arg)];
    strcpy(numberArg, arg);
    for (i = 0; i < strlen(numberArg); i++) {
        if (!isdigit(numberArg[i])) {
            return 0;
        }
    }
    return 1;
}

int isFileValid(char *arg) {
    size_t fileNameLength = strlen(arg);
    char fileName[fileNameLength];
    strcpy(fileName, arg);
    size_t dotIndex = fileNameLength - 4;
    return fileName[dotIndex] == '.' && fileName[dotIndex + 1] == 't' && fileName[dotIndex + 2] == 'x' &&
           fileName[dotIndex + 3] == 't';
}

