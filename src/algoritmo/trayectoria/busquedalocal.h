

#pragma once


#include <limits>
#include <stdio.h>
#include <vector>
#include "../../util/random.h"
#include "../../problema/representacion/base/base.h"
#include "../metodo.h"



namespace MH
{
namespace Trayectoria
{

	template <typename Problema>
	class BusquedaLocal : public MH::Metodo<Problema>
	{

		typedef typename Problema::Solucion Solucion;
		typedef typename Problema::Poblacion Poblacion;

	public:

		BusquedaLocal(
				Problema& pinst,
				RNG& prng )
			: MH::Metodo<Problema>( pinst, prng )
		{
			this->inst = pinst;
		}


		Solucion operator()()
		{
			Solucion actual, vecina;

			actual.ValidReset();
			actual.Randomizar( this->rng );
			float actual_fitness = this->inst.Fitness( actual );
			float vecina_fitness = std::numeric_limits<float>::max();
			float prev_mejor = -1;

			//std::vector<Solucion> vec;
			Poblacion vec;

			bool condicion_parada = false;
			int i = 0;

			while( !condicion_parada )
			{
				actual.GenerarVecinos( vec );
				float curr_fitness = 0;
				for( auto it : vec )
				{
					curr_fitness = this->inst.Fitness( it );
					//printf("currfitness:%f\n", curr_fitness );
					if( curr_fitness < vecina_fitness )
					{
						vecina_fitness = curr_fitness;
						vecina = it; // la mejor
						// la PRIMERA mejor: break;
					}
				}
				vec.clear();


				if( vecina_fitness >= actual_fitness )
				{
					condicion_parada = true;
				}
				else
				{
					actual_fitness = vecina_fitness;
					actual = vecina;
				}
			}

			actual.CalcularFitness( this->inst );
			return actual;
		}
	};

}
}
