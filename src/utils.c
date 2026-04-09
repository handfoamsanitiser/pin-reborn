#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "platform.h"

long long timerFreq = 0;
long long startTime = 0;

#ifdef PLATFORM_WINDOWS
#include <windows.h>

long long timer_freq_c(void) {
	LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li)) {
		printf("QueryPerformanceFrequency failed!\n");
	} else {
		printf("QPF works!");
	}
	return li.QuadPart;
}

long long timer_time_c(void) {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart;
}

double get_time(void) {
	return (double)timer_time_c() / (double)timerFreq;
}

void sleep_c(double ms) {
	if (ms < 0.0) return;
	Sleep((unsigned long)ms); // this is how raylib does it idk
}

#else
#include <unistd.h>

double timer_freq_c(void) {
	// TODO
}

long long timer_time_c(void) {
    // TODO
}

void sleep_c(double ms) {
	usleep(ms * 1000.0);
}

#endif

const char* read_file_to_string(const char* filePath) {
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