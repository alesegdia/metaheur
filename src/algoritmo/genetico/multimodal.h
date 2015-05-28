


#pragma once


#include <limits>
#include <stdio.h>
#include <vector>
#include "../../util/random.h"
#include "../../problema/representacion/base/base.h"
#include "../metodo.h"


namespace MH
{
namespace Genetico
{

	template <typename Problema>
	class Multimodal : public MH::Metodo<Problema>
	{

	private:

		typedef typename Problema::Solucion Solucion;
		//typedef typename Problema::Poblacion Poblacion;
		typedef std::vector<Solucion> Poblacion;

		int numIteraciones;
		int maximoPoblacion;
		int numGanadores;
		int numGanadoresNicho;
		float sigma;
		float probMutacionPorGen;
		float probCruce;

	public:

		Multimodal(
				Problema& pinst,
				RNG& prng,
				int itSM,
				int maxPob,
				int numGan,
				int numGanNicho,
				float sigma,
				float probCruce,
				float probMutGen)
			: MH::Metodo<Problema>( pinst, prng )
		{
			this->numIteraciones = itSM;
			this->maximoPoblacion = maxPob;
			this->numGanadores = numGan;
			this->numGanadoresNicho = numGanNicho;
			this->probMutacionPorGen = probMutGen;
			this->probCruce = probCruce;
			this->sigma = sigma;
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

			int iteraciones = numIteraciones;
			bool condicion_salida = false;
			Solucion mejor = this->MejorSolucion( poblacionActual );

			while( iteraciones >= 0 )
			{

				// ESTABLECEMOS NICHOS PONIENDO A CERO LOS QUE VAYAMOS A DESHECHAR
				this->OrdenarPoblacion( poblacionActual, true );
				//printf("\n\n");
				//this->DebugPoblacion( poblacionActual );
				for( int i = 0; i < poblacionActual.size(); i++ )
				{
					int numGan = 1;
					if( poblacionActual[i].GetFitness() > 0 )
					{
						for( int j = 0; j < poblacionActual.size(); j++ )
						{
							if( i != j )
							{
								//printf("DISTEUC: %f\n", poblacionActual[i].DistanciaEuclidea( poblacionActual[j] ) );
								if( poblacionActual[j].GetFitness() > 0 &&
										poblacionActual[i].DistanciaEuclidea( poblacionActual[j] ) < sigma )
								{
									if( numGan < numGanadoresNicho )
									{
										numGan++;
									}
									else
									{
										//printf("WE!\n");
										poblacionActual[j].SetFitness( 0 );
									}
								}
							}
						}
						//printf("\n");
					}
				}

				//printf("==============\nANTES\n");
				//this->DebugPoblacion( poblacionActual );
				// BORRAMOS LOS SOBRANTES DE CADA NICHO
				auto newend = std::remove_if( poblacionActual.begin(), poblacionActual.end(),
						[]( const Solucion& sol ) -> bool { return sol.GetFitness() == 0.f; } );
				poblacionActual.erase( newend, poblacionActual.end() );

				//printf("==============\nDESPUES\n");
				//this->DebugPoblacion( poblacionActual );

				// CRUZAMOS POR TORNEO, REUSANDO HIJOS
				while( poblacionActual.size() < maximoPoblacion )
				{

					Solucion padre1 = this->SeleccionTorneo( poblacionActual, numGanadores );
					if( this->rng.uniform() < probCruce )
					{
						Solucion padre2 = this->SeleccionTorneo( poblacionActual, numGanadores );

						Solucion hijo;
						hijo.Cruzar( padre1, padre2, this->rng );
						//for( int i = 0; i < Problema::PROB_SIZE; i++ )
						//{
							if( this->rng.uniform() > probMutacionPorGen )
							{
								hijo.Mutar( this->rng );
							}
						//}
						hijo.CalcularFitness( this->inst );
						poblacionActual.push_back( hijo );
					}
					else
					{
						poblacionActual.push_back( padre1 );
					}

				}

				// COMPROBAMOS SI HEMOS MEJORADO
				Solucion nueva_mejor = this->MejorSolucion( poblacionActual );
				if( nueva_mejor.GetFitness() < mejor.GetFitness() )
				{
					//nueva_mejor.Debug();
					mejor = nueva_mejor;
					//printf("MEJORA!, %f\n", mejor.GetFitness());
					iteraciones = numIteraciones;
					// iteraciones = TOTAL_DE_ITERACIONES? reset counter vamos, o algo asi
					// ^-> PROBAR!!
				}
				else
				{
					iteraciones--;
				}

			}


			/*
			this->OrdenarPoblacion( poblacionActual );
			printf("\n\n");
			for( auto it : poblacionActual )
			{
				it.Debug();
			}
			*/

			return mejor;
		}

	};

}
}
