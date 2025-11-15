#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PADDING_CHAR '#'

// Preenche uma string com caractere de preenchimento
void padString(char *str, size_t targetLength) {
    size_t len = strlen(str);
    for (size_t i = len; i < targetLength; i++)
        str[i] = PADDING_CHAR;
    str[targetLength] = '\0';
}

#endif