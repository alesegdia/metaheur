
#pragma once

#include <stdio.h>

#include <vector>
#include <iostream>
#include <fstream>

#include "base/base.h"
#include "base/arraybase.h"

template <size_t PROBSIZE, typename Problema>
class Orden : public RepresentacionBase<Problema>
{

	typedef int gen_t;
	typedef ArrayBase<gen_t, PROBSIZE> ArrayBaseInst;

public:

	static constexpr size_t PROB_SIZE = PROBSIZE;

	class Solucion :
		public ArrayBaseInst,
		public RepresentacionBase<Problema>::Solucion
	{

	public:

		Solucion( const Solucion& other ) : ArrayBaseInst( other ){
			this->fitness = other.GetFitness();
		};
		Solucion() : ArrayBaseInst()
		{
			//ValidReset();
		}

		Solucion( int sol[] )
		{
			for( int i = 0; i < Problema::PROB_SIZE; i++ )
			{
				ArrayBaseInst::data[i] = sol[i];
			}
		}

		virtual ~Solucion()
		{

		}

		Solucion& operator=( const Solucion &rhs )
		{
			ArrayBaseInst::operator=( rhs );
			this->fitness = rhs.GetFitness();
		}

		static bool Comparar( const Solucion& sol1,
				const Solucion& sol2 )
		{
			return sol1.fitness > sol2.fitness;
		}

		void Debug() const
		{
			printf("fitness: %f, ", this->fitness );
			ArrayBaseInst::Debug();
		}

		void ValidReset( )
		{
			for( int i = 0; i < PROBSIZE; i++ )
			{
				ArrayBaseInst::data[i] = i;
			}
		}

		void GenerarVecinos( ::std::vector<Solucion>& vec )
		{
			Solucion tmp;
			static int i = 0;
			for( int i = 0; i < PROBSIZE; i++ )
			{
				for( int j = i+1; j < PROBSIZE; j++ )
				{
					tmp.Copy( (*this) );
					vec.push_back( tmp );
					vec.back().Swap( i, j );
				}
			}
		}

		void Set( int el, int val )
		{
			ArrayBaseInst::data[el] = val;
		}

		void SetVal( gen_t val )
		{
			for( int i = 0; i < PROBSIZE; i++ )
			{
				ArrayBaseInst::data[i] = val;
			}
		}

		void Mutar( RNG& rng )
		{
			int r1, r2;
			r1 = rng.uniform( 0, PROBSIZE-1 );
			r2 = rng.uniform( 0, PROBSIZE-1 );
			ArrayBaseInst::Swap( r1, r2 );
		}

		void Mutar( RNG& rng, int& p1, gen_t& v1, int& p2, gen_t& v2 )
		{
			p1 = rng.uniform( 0, PROBSIZE-1 );
			p2 = rng.uniform( 0, PROBSIZE-1 );
			ArrayBaseInst::Swap( p1, p2 );
			v1 = this->data[p1];
			v2 = this->data[p2];
		}

		void Randomizar( RNG& rng, int iteraciones = PROBSIZE )
		{
			int r1, r2;
			ValidReset();

			for( int i = 0; i < iteraciones; i++ )
			{
				Mutar( rng );
			}

			//out.Debug();
			//std::cout << "fitness: " << Fitness( out ) << "\n\n";
		}

		void Cruzar( const Solucion& sol1, const Solucion& sol2, RNG& rng )
		{
			int r1, r2;
			r1 = rng.uniform( 0, PROBSIZE-1 );
			r2 = rng.uniform( 0, PROBSIZE-1 );
			if( r1 > r2 )
			{
				std::swap( r1, r2 );
			}

			for( int i = r1; i <= r2; i++ )
			{
				ArrayBaseInst::data[i] = sol1.data[i];
			}

			std::vector<int> resto;
			for( int i = (r2+1)%PROBSIZE; i != r1; i = ( i + 1 ) % PROBSIZE )
			{
				resto.push_back( sol1.Get( i ) );
			}

			int out_index = r2;
			for( int i = 0; i < PROBSIZE ; i++ )
			{
				// candidato, entra si esta en resto
				int candidato = sol2.Get( i );
				// recorremos el resto
				for( int j = 0; j < resto.size() ; j++ )
				{
					// si lo encontramos en el resto
					if( resto[j] == candidato )
					{
						// avanzamos el indice
						out_index = ( out_index + 1 ) % PROBSIZE;
						//printf("copiar %d a la posicion %d\n", candidato, out_index );
						ArrayBaseInst::data[out_index] = candidato;
						break;
					}
				}
			}
		}

		int DistanciaHamming( const Solucion& other )
		{
			int dist = 0;
			for( int i1 = 0, i2 = 1; i1 < PROBSIZE; i1++, i2 = (i2+1) % PROBSIZE )
			{
				bool ok = false;
				for( int j1 = 0, j2 = 1; j1 < PROBSIZE && !ok ; j1++, j2 = (j2+1) % PROBSIZE )
				{
					if( other.Get(i1) == this->Get(j1) && other.Get(i2) == this->Get(j2) )
					{
						ok = true;
						break;
					}
				}
				if( !ok ) dist++;
			}

			return dist;
		}

		float DistanciaEuclidea( Solucion& other )
		{
			float sum = 0;
			for( int i = 0; i < PROB_SIZE; i++ )
			{
				float factor = pow( other.data[i] - this->data[i], 2 );
				sum += factor;
			}
			return sqrt( sum );
		}

	};

	typedef ::std::vector<Solucion> Poblacion;
	virtual float Fitness( const Solucion& sol ){
		printf("hey");
	}

};


