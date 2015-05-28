

#pragma once


#include <limits>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "../../util/random.h"
#include "../../problema/representacion/base/base.h"
#include "../metodo.h"


namespace MH
{
namespace Genetico
{

	template <typename Problema>
	class Basico : public MH::Metodo<Problema>
	{

	private:

		typedef typename Problema::Solucion Solucion;
		//typedef typename Problema::Poblacion Poblacion;
		typedef std::vector<Solucion> Poblacion;

		int numIteraciones;
		int maximoPoblacion;
		int numGanadores;
		float probCruce;
		float probMutacion;

	public:

		Basico(
				Problema& pinst,
				RNG& prng,
				int itSM,
				int maxPob,
				int numGan,
				float pcruce,
				float pmutar )
			: MH::Metodo<Problema>( pinst, prng )
		{
			this->numIteraciones = itSM;
			this->maximoPoblacion = maxPob;
			this->numGanadores = numGan;
			this->probCruce = pcruce;
			this->probMutacion = pmutar;
		}

		Solucion operator()()
		{
			Poblacion poblacionActual;
			for( int i = 0; i < maximoPoblacion; i++ )
			{
				poblacionActual.push_back(Solucion());
				poblacionActual.back().ValidReset();
				poblacionActual.back().Randomizar( this->rng );
				poblacionActual.back().CalcularFitness( this->inst );
				//poblacionActual.back().Debug();
			}

			Solucion mejor_actual, mejor_anterior;
			int iteraciones_sin_mejorar = numIteraciones;
			while( iteraciones_sin_mejorar > 0 )
			{
				Poblacion poblacionNueva;

				// añadir ganadores a poblacionNueva
				while( poblacionNueva.size() < maximoPoblacion )
				{
					Solucion padre1 = this->SeleccionTorneo( poblacionActual, numGanadores );
					Solucion padre2 = this->SeleccionTorneo( poblacionActual, numGanadores );

					if( probCruce > this->rng.uniform() )
					{
						Solucion hijo;
						hijo.Cruzar( padre1, padre2, this->rng );
						if( probMutacion < this->rng.uniform() )
						{
							hijo.Mutar( this->rng );
						}
						hijo.CalcularFitness( this->inst );
						poblacionNueva.push_back( hijo );
					}
					else
					{
						poblacionNueva.push_back( padre1 );
						poblacionNueva.push_back( padre2 );
					}
				}

				Solucion elite, mejor_actual, mejor_nueva;
				mejor_actual = this->MejorSolucion( poblacionActual );
				mejor_nueva = this->MejorSolucion( poblacionNueva );
				if( mejor_nueva.GetFitness() >= mejor_actual.GetFitness() )
				{
					elite = mejor_actual;
					//printf("...nomejora..\n");
					iteraciones_sin_mejorar--;
				}
				else
				{
					elite = mejor_nueva;
					//printf("MEJORA! %f\n", elite.GetFitness() );
					iteraciones_sin_mejorar = numIteraciones;
				}

				poblacionActual.clear();
				poblacionActual = poblacionNueva;
				poblacionActual.push_back( elite );
			}


			//this->OrdenarPoblacion( poblacionActual, false );
			//this->DebugPoblacion( poblacionActual );
			//printf("\n\n");

			return this->MejorSolucion( poblacionActual );
		}

		float DesviacionTipica( Poblacion& pob )
		{
			return 0.f;
		}

	};

}
}
