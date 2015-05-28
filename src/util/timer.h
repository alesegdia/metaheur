
#pragma once

#include <time.h>

class Timer
{

	time_t start;

public:

	Timer(){}

	void Start()
	{
		time(&start);
	}

	time_t Get()
	{
		time_t t;
		time( &t );
		return ( t - start );
	}

};
