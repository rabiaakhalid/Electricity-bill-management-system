#include <stdio.h>
#include "structs.h"

/* Throws away everything left on the current input line. */
void clearBuffer(void) {
    int ch;

    ch = getchar();
    while (ch != '\n' && ch != EOF) {
        ch = getchar();
    }
}

/* Keeps asking until the user types a valid whole number. */
int readInt(void) {
    int value, result;

    while (1) {
        result = scanf("%d", &value);
        clearBuffer();
        if (result == 1) {
            return value;
        }
        printf("Invalid input. Please enter a whole number: ");
    }
}

/* Keeps asking until the user types a valid number (with or without decimals). */
float readFloat(void) {
    float value;
    int result;

    while (1) {
        result = scanf("%f", &value);
        clearBuffer();
        if (result == 1) {
            return value;
        }
        printf("Invalid input. Please enter a number: ");
    }
}

/* Reads a whole line (spaces allowed) into s. Keeps asking if the line is empty. */
void readString(char s[], int size) {
    int i, foundNewline;

    do {
        fgets(s, size, stdin);

        foundNewline = 0;
        for (i = 0; s[i] != '\0'; i++) {
            if (s[i] == '\n') {
                s[i] = '\0';          /* remove the newline */
                foundNewline = 1;
                break;
            }
        }

        if (foundNewline == 0) {
            clearBuffer();            /* input was too long, drop the extra part */
        }

        if (s[0] == '\0') {
            printf("Input cannot be empty. Try again: ");
        }
    } while (s[0] == '\0');
}
