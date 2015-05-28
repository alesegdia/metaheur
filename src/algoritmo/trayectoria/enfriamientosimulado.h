
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
	class EnfriamientoSimulado : public MH::Metodo<Problema>
	{

		typedef typename Problema::Solucion Solucion;
		typedef typename Problema::Poblacion Poblacion;

		typedef void (EnfriamientoSimulado::*MetodoEnfriamiento)(float&, int);

		float mu;
			// tanto por uno
		float phi;
			// probabilidad de aceptar una solucion peor en un phi por uno que la inicial

		int maxGenerados;
		float porcentajeAceptados;

		float parametroEnfriamiento;
		MetodoEnfriamiento metodoEnfriamiento;


	public:

		EnfriamientoSimulado(
				Problema& pinst,
				RNG& prng,
				float mu,
				float phi,
				int maxGenerados,
				float porcentajeAceptados
				)
			: MH::Metodo<Problema>( pinst, prng )
		{
			this->mu = mu;
			this->phi = phi;
			this->maxGenerados = maxGenerados;
			this->porcentajeAceptados = porcentajeAceptados;
		}

		void DescensoExponencial( float& T, int iteracion )
		{
			T = parametroEnfriamiento * T;
		}

		void Boltzman( float& T, int iteracion )
		{
			T = parametroEnfriamiento / ( 1 + iteracion );
		}

		Solucion operator()()
		{
			Solucion actual, candidata, mejor;
			actual.ValidReset();
			actual.Randomizar( this->rng );
			actual.CalcularFitness( this->inst );
			mejor.ValidReset();
			mejor.Randomizar( this->rng );
			mejor.CalcularFitness( this->inst );

			bool condicion_parada = false;
			int generados, aceptados, maxAceptados, iteracion;
			maxAceptados = maxGenerados * porcentajeAceptados;
			iteracion = 0;

			float temperaturaActual = ( mu / ( -log( phi ))) * actual.GetFitness() ;
			parametroEnfriamiento = temperaturaActual;
			while( !condicion_parada )
			{
				condicion_parada = true;
				generados = aceptados = 0;

				while( generados < maxGenerados &&
						aceptados < maxAceptados )
				{
					generados++;
					candidata = actual;
					candidata.Mutar( this->rng );
					candidata.CalcularFitness( this->inst );
					if( candidata.GetFitness() < mejor.GetFitness() )
						mejor = candidata;
					float deltaTemp = candidata.GetFitness() - actual.GetFitness();
					float prob = exp( - ( deltaTemp / temperaturaActual ));

					if( deltaTemp < 0 || this->rng.uniform() < prob )
					{
						aceptados++;
						actual = candidata;
						condicion_parada = false;
					}
				}
				iteracion++;
				Boltzman( temperaturaActual, iteracion );
			}

			return actual;
		}
	};

}
}
