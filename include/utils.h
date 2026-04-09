#pragma once

extern long long timerFreq;
extern long long startTime;

void sleep_c(double ms);
long long timer_freq_c(void);
long long timer_time_c(void);
double get_time(void);

const char* read_file_to_string(const char* fileName);