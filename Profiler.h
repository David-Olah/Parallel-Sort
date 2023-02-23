#pragma once

#include <time.h>

#define MAX_PROFILES 10
#define MAX_PROFILE_TIMERS 100
#define PROFILE_NAME_LEN 16

typedef struct ProfileData {
	char name[PROFILE_NAME_LEN];
	int readings;
	clock_t total;
	clock_t min;
	clock_t max;
	clock_t avg;
} ProfileData;

int begin_profile(char* name);
void end_profile(int timer_id);
void get_profiles(ProfileData** profiles, int* profile_count);
void init_profiler();