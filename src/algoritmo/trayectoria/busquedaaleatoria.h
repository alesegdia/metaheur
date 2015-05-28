
#pragma once

#include <limits>
#include <stdio.h>
#include "../../util/random.h"
#include "../../problema/representacion/base/base.h"
#include "../metodo.h"



namespace MH
{
namespace Trayectoria
{

	template <typename Problema>
	class BusquedaAleatoria : public MH::Metodo<Problema>
	{

		typedef typename Problema::Solucion Solucion;

	private:

		int iteraciones;


	public:

		BusquedaAleatoria(
				Problema& pinst,
				RNG& prng,
				int iteraciones	)
			: MH::Metodo<Problema>( pinst, prng )
		{
			this->iteraciones = iteraciones;
		}

		Solucion operator()()
		{
			int limite = iteraciones * Problema::PROBLEM_SIZE;
			Solucion tmp;
			Solucion mejor;
			mejor.SetFitness( std::numeric_limits<float>::max() );
			while( limite )
			{
				tmp.ValidReset();
				tmp.Randomizar( this->rng );
				tmp.CalcularFitness( this->inst );

				if( tmp.GetFitness() < mejor.GetFitness() )
				{
					mejor = tmp;
					mejor.Debug();
				}

				limite--;

			}

			return mejor;
		}

	};


}
}
