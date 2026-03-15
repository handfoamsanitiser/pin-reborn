#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* BASE_SHADER_PATH = "something";

const char* ReadFileToString(const char* fileName) {
    char fullShaderPath[32];
    for (int i = 0; i < sizeof(fullShaderPath); ++i) {
        fullShaderPath[i] = '\0';
    }

    strcat(fullShaderPath, BASE_SHADER_PATH);
    strcat(fullShaderPath, fileName);
    //printf("%s\n", fullShaderPath);

    FILE* file = fopen(fullShaderPath, "r");

    if (file == NULL) {
        printf("Failed to read file.");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* string = (char*)malloc(length * sizeof(char) + 1);

    for (int i = 0; i < length; ++i) {
        string[i] = fgetc(file);
    }
    string[length] = '\0';

    fclose(file);
    return string;
}