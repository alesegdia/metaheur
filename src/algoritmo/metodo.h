
#pragma once

#include "../problema/representacion/base/base.h"
#include <algorithm>
#include <vector>

namespace MH
{
template <typename Problema>
class Metodo
{

protected:

	typedef typename Problema::Solucion Solucion;
	typedef std::vector<Solucion> Poblacion;

	Problema& inst;
	RNG& rng;

public:

	Metodo(
			Problema& pinst,
			RNG& prng)
		: inst( pinst ),
			rng( prng )
	{

	}

	virtual ~Metodo()
	{

	}

	virtual typename Problema::Solucion operator()()
	{

	}

	void DebugPoblacion( const Poblacion& pob )
	{
		for( auto sol : pob )
		{
			sol.Debug();
		}
	}

	Solucion MejorSolucion( Poblacion& poblacion )
	{
		int mejor = 0;
		for ( int i = 1; i < poblacion.size(); i++ )
		{
			if( poblacion[i].GetFitness() < poblacion[mejor].GetFitness() )
			{
				mejor = i;
			}
		}

		return poblacion[mejor];
	}

	void SetSeed( uint32_t pseed )
	{
		rng.seed( pseed );
	}

	int MejorSolucionIndex( Poblacion& poblacion )
	{
		int mejor = 0;
		for( int i = 1; i < poblacion.size(); i++ )
		{
			if( poblacion[i].GetFitness() < poblacion[mejor].GetFitness() )
			{
				mejor = i;
			}
		}

		return mejor;
	}

	Solucion SeleccionTorneo( Poblacion& poblacion, int numGanadores )
	{
		Solucion ret;
		int mejor_fitness = std::numeric_limits<int>::max();

		for( int i = 0; i < numGanadores; i++ )
		{
			int elegido = this->rng.uniform( 0, poblacion.size() - 1 );
			float fitness_actual = poblacion[elegido].GetFitness();
			if( fitness_actual < mejor_fitness )
			{
				mejor_fitness = fitness_actual;
				ret = poblacion[elegido];
			}
		}

		return ret;
	}
	


	void OrdenarPoblacion( Poblacion& pob, bool asc = false )
	{
		if( asc == false )
		{
			std::sort( pob.begin(), pob.end(),
			[]( const typename Problema::Solucion& s1,
						const typename Problema::Solucion& s2 ) -> bool
					{ return s1.GetFitness() > s2.GetFitness(); });
		}
		else
		{
			std::sort( pob.begin(), pob.end(),
			[]( const typename Problema::Solucion& s1,
						const typename Problema::Solucion& s2 ) -> bool
					{ return s1.GetFitness() < s2.GetFitness(); });
		}

	}

};

struct MetodoConfig
{

};

}
