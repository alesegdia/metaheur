

#pragma once

#include "representacion/orden.h"
#include "../util/matriz.h"
#include <iostream>

template <size_t PROBSIZE>
class TSP : public Orden<PROBSIZE, TSP<PROBSIZE>>
{

public:

typedef typename Orden<PROBSIZE, TSP<PROBSIZE>>::Solucion Solucion;
typedef std::vector<Solucion> Poblacion;

private:

	Matriz<float> distancias;
	Matriz<float> heuristicas;


public:

	static constexpr int PROBLEM_SIZE = PROBSIZE;

	void LoadFromFile( const char* path )
	{
		::std::ifstream file( path );

		distancias.Alloc( PROBSIZE, PROBSIZE );
		heuristicas.Alloc( PROBSIZE, PROBSIZE );

		for( int i = 0; i < PROBSIZE; i++ )
		{
			for( int j = 0; j < PROBSIZE; j++ )
			{
				distancias.Set( i, j, 0 );
				heuristicas.Set( i, j, 0 );
			}
		}

		float ciudades[PROBSIZE][2];
		int tmp;
		float c1, c2;
		for( int i = 0; i < PROBSIZE; i++ )
		{
			file >> tmp;
			file >> ciudades[i][0];
			file >> ciudades[i][1];
			//printf("%d) %f, %f\n", tmp, ciudades[i][0], ciudades[i][1] );
		}

		for( int i = 0; i < PROBSIZE; i++ )
		{
			for( int j = i+1; j < PROBSIZE; j++ )
			{
				float pow0, pow1, dist;
				pow0 = pow( ciudades[i][0] - ciudades[j][0], 2 );
				pow1 = pow( ciudades[i][1] - ciudades[j][1], 2 );
				dist = sqrt( pow0 + pow1 );
				distancias.Set( i, j, dist );
				distancias.Set( j, i, dist );
				heuristicas.Set( i, j, 1.f/dist );
				heuristicas.Set( j, i, 1.f/dist );
			}
		}

	}

	void Debug()
	{
		distancias.Debug();
		heuristicas.Debug();
		printf("\n");
		printf("\n");
	}

	float Fitness( const Solucion& sol )
	{
		float fitness = 0;
		int i1, i2;
		i1 = 0; i2 = 1;
		for( ; i1 < PROBSIZE; i1++, i2=(i2+1)%PROBSIZE )
		{
			fitness += distancias.Get( sol.Get(i1), sol.Get(i2) );
			//printf("%d,%d,%f\n", i1, i2, fitness);
		}
		return fitness;
	}

	float GetHeuristica( int i, int j )
	{
		return 1.f/distancias.Get( i, j );
	}

	Solucion Greedy( )
	{
		int solucion[PROBSIZE];
		for( int i = 0; i < PROBSIZE; i++ )
		{
			solucion[i] = i;
		}

		for( int i = 1; i < PROBSIZE; i++ )
		{
			// por defecto el siguiente
			int min_dist, min_ciudad;
			min_ciudad = i;
			min_dist = distancias.Get( solucion[i-1], solucion[min_ciudad] );

			for( int j = i+1; j < PROBSIZE; j++ )
			{
				float dist = distancias.Get( solucion[i-1], solucion[j] );
				if( dist < min_dist )
				{
					//printf("bleh");
					min_dist = dist;
					min_ciudad = j;
				}
			}

			std::swap( solucion[i], solucion[min_ciudad] );
			//solucion[i] = min_ciudad;
		}

		Solucion sol( solucion );
		sol.CalcularFitness( *this );
		return sol;
	}

};



