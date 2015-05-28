

#pragma once

#include <random>
#include <stdint.h>

class RNG
{

private:

	std::mt19937 gen;
	std::uniform_int_distribution<int> intdist;
	std::uniform_real_distribution<float> floatdist;

public:

	RNG() : floatdist( 0.f, 1.f ) {}

	void seed( uint32_t seed )
	{
		gen.seed( seed );
	}

	int uniform( int low, int high )
	{
		std::uniform_int_distribution<>::param_type newparams( low, high );
		intdist.param( newparams );
		return intdist( gen );
	}

	float uniform( )
	{
		return floatdist( gen );
	}



};

