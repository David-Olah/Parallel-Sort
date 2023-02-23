#include <string.h>
#include <stdio.h>
#include <Windows.h>

#include "Profiler.h"

typedef struct Timer {
	clock_t start_time;
	int profile_id;
} Timer;

ProfileData profile_list[MAX_PROFILES];
Timer timer_list[MAX_PROFILE_TIMERS];
int next_profile = 0;
int next_timer = 0;

HANDLE profiler_sem;

int begin_profile(char* name)
{
	WaitForSingleObject(profiler_sem, INFINITE);

	if (next_timer >= MAX_PROFILE_TIMERS)
	{
		ReleaseSemaphore(profiler_sem, 1, NULL);
		printf("Too many timers.\n");
		return -1;
	}

	int id = -1;
	for (int i = 0; i <= next_profile; i++)
		if (strncmp(profile_list[i].name, name, PROFILE_NAME_LEN) == 0)
		{
			id = i;
			break;
		}

	if (id == -1)
	{
		if (next_profile >= MAX_PROFILES)
		{
			ReleaseSemaphore(profiler_sem, 1, NULL);
			printf("Too many profiles.\n");
			return -1;
		}

		id = next_profile++;

		ProfileData* profile = profile_list + id;
		profile->readings = 0;
		profile->total = 0;
		profile->min = INFINITE;
		profile->max = 0;
		profile->avg = 0;
		memcpy(profile->name, name, PROFILE_NAME_LEN);
	}

	int timer_id = next_timer++;
	Timer* timer = timer_list + timer_id;
	timer->profile_id = id;
	timer->start_time = clock();

	ReleaseSemaphore(profiler_sem, 1, NULL);
	return timer_id;
}

void end_profile(int timer_id)
{
	clock_t end_time = clock();

	WaitForSingleObject(profiler_sem, INFINITE);

	Timer* timer = timer_list + timer_id;
	ProfileData* profile = profile_list + timer->profile_id;

	clock_t time = end_time - timer->start_time;
	profile->total += time;
	if (time < profile->min || profile->min == INFINITE)
		profile->min = time;
	if (time > profile->max)
		profile->max = time;
	profile->avg = profile->total / ++profile->readings;

	ReleaseSemaphore(profiler_sem, 1, NULL);
}

void get_profiles(ProfileData** profiles, int* profileCount)
{
	*profiles = profile_list;
	*profileCount = next_profile;
}

void init_profiler()
{
	profiler_sem = CreateSemaphore(NULL, 1, 1, NULL);
}