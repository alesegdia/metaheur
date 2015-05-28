
#pragma once

#include "representacion/orden.h"
#include "../util/matriz.h"
#include <iostream>

template <size_t PROBSIZE>
class QAP : public Orden<PROBSIZE, QAP<PROBSIZE>>
{

public:

typedef typename Orden<PROBSIZE, QAP<PROBSIZE>>::Solucion Solucion;
typedef std::vector<Solucion> Poblacion;

private:

	Matriz<int> distancias;
	Matriz<int> flujo;

	void ParseMatriz( Matriz<int>& mat, std::ifstream& file )
	{
		int tmp;
		for( int i = 0; i < PROBSIZE; i++ )
		{
			for( int j = 0; j < PROBSIZE; j++ )
			{
				file >> tmp;
				mat.Set( i, j, tmp );
			}
		}
	}


public:

	static constexpr int PROBLEM_SIZE = PROBSIZE;

	void LoadFromFile( const char* path )
	{
		::std::ifstream file( path );

		int tmp;

		file >> tmp;
		printf(" tamanio: %d\n", tmp );
		distancias.Alloc( PROBSIZE, PROBSIZE );
		flujo.Alloc( PROBSIZE, PROBSIZE );

		ParseMatriz( distancias, file );
		ParseMatriz( flujo, file );

	}

	void Debug()
	{
		flujo.Debug();
		printf("\n");
		distancias.Debug();
		printf("\n");
		printf("\n");
	}

	float Fitness( const Solucion& sol )
	{
		float sum = 0;
		for( int i = 0; i < PROBSIZE; i++ )
		{
			for( int j = 0; j < i; j++ )
			{
				sum += flujo.Get( i, j ) * distancias.Get( sol.Get( i ), sol.Get( j ));
			}

			for( int j = i+1; j < PROBSIZE; j++ )
			{
				sum += flujo.Get( i, j ) * distancias.Get( sol.Get( i ), sol.Get( j ));
			}
		}

		return sum;
	}

	Solucion Greedy( )
	{
		Solucion sol;
		sol.Reset( -1 );

		int sumflujos[PROBSIZE];
		int sumdists[PROBSIZE];
		for( int col = 0; col < PROBSIZE; col++ )
		{
			sumflujos[col] = flujo.SumCol( col );
			sumdists[col] = distancias.SumCol( col );
		}

		for( int i = 0; i < PROBSIZE; i++ )
		{
			int mayor_flujo, mayor_unidad, menor_dist, menor_loc;
			int actual_dist, actual_flujo;

			mayor_flujo = -1;
			mayor_unidad = -1;
			menor_dist = ::std::numeric_limits<int>::max();
			menor_loc = -1;

			// buscamos los mejores
			for( int col = 0; col < PROBSIZE;  col++ )
			{
				actual_dist = sumdists[col];
				actual_flujo = sumflujos[col];
				if( actual_flujo > mayor_flujo )
				{
					mayor_flujo = actual_flujo;
					mayor_unidad = col;
				}
				if( actual_dist < menor_dist )
				{
					menor_dist = actual_dist;
					menor_loc = col;
				}
			}

			sol.Set( mayor_unidad, menor_loc );
			sumflujos[mayor_unidad] = -1;
			sumdists[menor_loc] = ::std::numeric_limits<int>::max();

		}

		sol.CalcularFitness( *this );
		return sol;
	}

};

