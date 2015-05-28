
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
	class CHC : public MH::Metodo<Problema>
	{

	private:

		typedef typename Problema::Solucion Solucion;
		typedef std::vector<Solucion> Poblacion;

		int maximoPoblacion;
		int maximoReinicios;

	public:

		CHC(
				Problema& pinst,
				RNG& prng,
				int maximoPoblacion,
				int maximoReinicios)
			: MH::Metodo<Problema>( pinst, prng )
		{
			this->maximoPoblacion = maximoPoblacion;
			this->maximoReinicios = maximoReinicios;
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
			}

			bool condicion_parada = false;
			int umbral_cruce = Problema::PROB_SIZE/4;
			int veces = 0;
			float probMutacion = 0.1f;
			Solucion mejor_solucion_global;
			mejor_solucion_global.ValidReset();
			mejor_solucion_global.Randomizar( this->rng );
			mejor_solucion_global.CalcularFitness( this->inst );
			while( !condicion_parada )
			{
				Poblacion poblacionNueva, hijos, actualBarajada;
				bool hay_cruce = false;


				// BARAJAR POBLACION
				actualBarajada = poblacionActual;
				std::random_shuffle ( actualBarajada.begin(), actualBarajada.end() );

				// CRUZAR PAREJAS
				int numParejas = floor( actualBarajada.size() / 2 );
				int i = 0;
				for( ; numParejas > 0 && poblacionNueva.size() < maximoPoblacion; i += 2, numParejas-- )
				{
					if( actualBarajada[i].DistanciaHamming( actualBarajada[i+1] ) > umbral_cruce )
					{
						for( int i = 0;  i < 1; i++ )
						{
							Solucion nueva;
							nueva.Cruzar( actualBarajada[i], actualBarajada[i+1], this->rng );
							nueva.CalcularFitness( this->inst );
							hijos.push_back( nueva );
						}
						hay_cruce = true;
						numParejas--;
					}
				}

				if( !hay_cruce ) umbral_cruce--;

				OrdenarPoblacion( poblacionActual, true ); // ascendente
				OrdenarPoblacion( hijos, true ); // ascendente

				int sol_hija = 0;
				while( sol_hija < hijos.size() && hijos[sol_hija].GetFitness() < poblacionActual.back().GetFitness() )
				{
					sol_hija++;
				}

				poblacionNueva.insert(
						poblacionNueva.begin(),
						hijos.begin(),
						hijos.begin() + sol_hija
					);

				poblacionNueva.insert(
						poblacionNueva.end(),
						poblacionActual.begin(),
						poblacionActual.begin() + ( maximoPoblacion - poblacionNueva.size() + 1 )
						);

				condicion_parada = veces > maximoReinicios;
				if( umbral_cruce < 0 && !condicion_parada )
				{
					// REARRANQUE

					// Copiamos mejores de la poblacion que hemos generado
					poblacionActual.clear();
					OrdenarPoblacion( poblacionNueva, 1 );
					for( int i = 0; i < 1; i++ )
					{
						poblacionActual.push_back( poblacionNueva[i] );
					}

					// Rellenamos aleatorio el resto
					while( poblacionActual.size() < maximoPoblacion )
					{
						Solucion nueva;
						nueva.ValidReset();
						nueva.Randomizar( this->rng );
						nueva.CalcularFitness( this->inst );
						poblacionActual.push_back( nueva );
					}

					umbral_cruce = Problema::PROB_SIZE/4;
					veces++;
				}
				else if( hay_cruce )
				{
					poblacionActual = poblacionNueva;
				}

				Solucion mejor_solucion_actual = this->MejorSolucion( poblacionActual );
				if( mejor_solucion_actual.GetFitness() < mejor_solucion_global.GetFitness() )
				{
					mejor_solucion_global = mejor_solucion_actual;
				}

			}


			//printf("SOLFINAL!!\n");
			//OrdenarPoblacion( poblacionActual, 1 );
			//this->DebugPoblacion( poblacionActual );

			//printf("MEJOR: \n");
			//mejor_solucion_global.Debug();

			return mejor_solucion_global;
		}

		float DesviacionTipica( Poblacion& pob )
		{
			return 0.f;
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

		Solucion MejorSolucion( Poblacion& poblacion )
		{
			Solucion mejor;
			float mejor_fitness = std::numeric_limits<float>::max();
			for ( int i = 0; i < poblacion.size(); i++ )
			{
				int elegido = this->rng.uniform( 0, poblacion.size() - 1 );
				float fitness_actual = poblacion[elegido].GetFitness();
				if( fitness_actual < mejor_fitness )
				{
					mejor_fitness = fitness_actual;
					mejor = poblacion[elegido];
				}
			}

			return mejor;
		}

		void OrdenarPoblacion( Poblacion& pob, int n = 0 )
		{
			if( n == 0 )
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

}
}
