#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>

void sleep_c(float milliseconds) {
	Sleep(milliseconds);
}

#else
#include <unistd.h>

void sleep_c(float milliseconds) {
	// usleep is in microseconds, hence * 1000.0f
	usleep(milliseconds * 1000.0f);
}

#endif

const char* ReadFileToString(const char* filePath) {
    FILE* file = fopen(filePath, "r");

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

