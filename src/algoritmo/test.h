
#include <limits>
#include <stdio.h>
#include <stdint.h>
#include "../util/random.h"
#include "metodo.h"

template <typename Problema>
void test(
		Problema& def,
		MH::Metodo<Problema>& algo,
		int iteraciones,
		bool detalle=false)
{

	std::vector<uint32_t> seedvec;
	while( iteraciones > 0 )
	{
		seedvec.push_back( (uint32_t) iteraciones * 999 );
		iteraciones--;
	}


	test( def, algo, seedvec, detalle );
}

template <typename Problema>
void test(
		Problema& def,
		MH::Metodo<Problema>& algo,
		std::vector<uint32_t> seedvec,
		bool detalle = false
	)

{

	typename Problema::Solucion sol, mejor, peor;
	mejor.SetFitness( std::numeric_limits<float>::max() );
	peor.SetFitness( std::numeric_limits<float>::min() );

	float* fitness_sol = new float[seedvec.size()];

	float sumfitness=0;
	double sumsqfitness=0;

	int i = 0;
	for( auto seed : seedvec )
	{
		algo.SetSeed( seed );
		sol = algo();

		sumfitness += sol.GetFitness();
		fitness_sol[i] = sol.GetFitness();
		i++;

		if( sol.GetFitness() < mejor.GetFitness() )
		{
			mejor = sol;
		}

		if( sol.GetFitness() > peor.GetFitness() )
		{
			peor = sol;
		}
	}

	float media = sumfitness / seedvec.size();
	float varianza = 0;
	for( int i = 0; i < seedvec.size(); i++ )
	{
		float varianza_act = pow( fitness_sol[i] - media, 2 );
		varianza += varianza_act;
	}
	varianza /= seedvec.size();

	//mejor.Debug();
	printf( "mejor: %e\n", mejor.GetFitness() );
	printf( "peor: %e\n", peor.GetFitness() );
	printf("media: %e\n", media );
	printf( "desviacion tipica: %e\n\n", sqrt( varianza ) );

	if( detalle )
	{
		for( int i = 0; i < seedvec.size(); i++ )
		{
			printf( "SEMILLA: %lx, FITNESS: %e\n", seedvec[i], fitness_sol[i] );
		}
		printf("\n\n");
	}

	delete fitness_sol;
}
