

#pragma once


#include <limits>
#include <stdio.h>
#include <vector>
#include "../../util/random.h"
#include "../../util/timer.h"
#include "../../util/matriz.h"
#include "../../problema/representacion/base/base.h"
#include "../metodo.h"



namespace MH
{
namespace Hormigas
{

	template <typename Problema>
	class SH: public MH::Metodo<Problema>
	{

		typedef typename Problema::Solucion Solucion;
		typedef typename Problema::Poblacion Poblacion;

		Matriz<float> feromonas;
		Solucion solgreedy;
		float aporte_inicial, alfa, beta, rho;
		int minutos_limite, num_hormigas;

	public:

		SH(
				Problema& pinst,
				RNG& prng,
				int min_lim,
				int num_hor,
				float alfa_,
				float beta_,
				float rho_) :
			MH::Metodo<Problema>( pinst, prng ),
			feromonas( Problema::PROB_SIZE )
		{
			solgreedy = pinst.Greedy();
			aporte_inicial = 1.f / (((float)Problema::PROB_SIZE) * solgreedy.GetFitness());
			minutos_limite = min_lim;
			num_hormigas = num_hor;
			alfa = alfa_;
			beta = beta_;
			rho = rho_;
		}

		Solucion operator()()
		{
			Solucion mejor_global;
			mejor_global.SetFitness( std::numeric_limits<float>::max() );
			feromonas.Set( 0 );
			for( int i = 0; i < Problema::PROB_SIZE; i++ )
			{
				for( int j = i+1; j < Problema::PROB_SIZE; j++ )
				{
					SetFeromonas( i, j, aporte_inicial );
				}
			}

			Timer t;
			t.Start();
			while( t.Get() < minutos_limite * 60 )
			{
				EvaporarFeromonas();
				for( int i = 0; i < num_hormigas; i++ )
				{
					Solucion actual = CalcularCaminoHormiga( );
					AportarFeromonas( actual );
					if( actual.GetFitness() < mejor_global.GetFitness() )
					{
						//actual.Debug();
						//fflush(0);
						//printf("%f %d\n", actual.GetFitness(), t.Get() );
						fflush(0);
						mejor_global = actual;
					}
				}
			}

			mejor_global.Debug();
			return mejor_global;
		}

			// Calculamos la solución propuesta por una hormiga empleando una ruleta
			// proporcional, teniendo en cuenta la regla de transición para establecer
			// la probabilidad proporcional de cada ciudad en cada paso.
		Solucion CalcularCaminoHormiga( )
		{

			int sol[Problema::PROB_SIZE];
			for( int i = 0; i < Problema::PROB_SIZE; i++ )
			{
				sol[i]=i;
			}

			for( int i = 1; i < Problema::PROB_SIZE; i++ )
			{
				int ciudad_elegida = Problema::PROB_SIZE - 1;
				float total_aportacion = 0.f;
				float aportaciones[Problema::PROB_SIZE] = { 0 };

				for( int j = i; j < Problema::PROB_SIZE; j++ )
				{
					float fero, heur;
					fero = GetFeromonas( sol[i-1], sol[j] );
					heur = this->inst.GetHeuristica( sol[i-1], sol[j] );
					aportaciones[j] = pow( fero, alfa ) * pow( heur, beta );
					total_aportacion += aportaciones[j];
				}

				float unif = this->rng.uniform();
				float current = aportaciones[i] / total_aportacion;

				for( int j = i; j < Problema::PROB_SIZE-1; j++ )
				{
					if( unif < current )
					{
						ciudad_elegida = j;
						break;
					}
					aportaciones[j+1] /= total_aportacion;
					current += aportaciones[j+1];
				}

				std::swap( sol[i], sol[ciudad_elegida] );
			}

			Solucion ret( sol );
			ret.CalcularFitness( this->inst );
			return ret;
		}

		void EvaporarFeromonas( )
		{
			for( int i = 0; i < Problema::PROB_SIZE; i++ )
			{
				for( int j = i+1; j < Problema::PROB_SIZE; j++ )
				{
					float newfer = GetFeromonas( i, j ) * ( 1 - rho );
					SetFeromonas( i, j, newfer );
				}
			}
		}

		float CalcularAporte( float fitness )
		{
			return 1 / fitness;
		}

		void AportarFeromonas( const Solucion& sol )
		{
			// APORTE: aporta solo el camino
			float aporte = CalcularAporte( sol.GetFitness() );
			for( int i = 1; i < Problema::PROB_SIZE; i++ )
			{
				int f1, f2;
				f1 = sol.Get(i);
				f2 = sol.Get(i-1);
				float newfer = GetFeromonas( f1, f2 ) + aporte;
				SetFeromonas( f1, f2, newfer );
			}

			float newfer = GetFeromonas( sol.Get(0), sol.Get(Problema::PROB_SIZE-1));
			SetFeromonas( sol.Get(0), sol.Get( Problema::PROB_SIZE - 1 ), newfer + aporte );
		}

		float GetFeromonas( int i, int j )
		{
			return feromonas.Get( i, j );
		}

		void SetFeromonas( int i, int j, float val )
		{
			feromonas.Set( i, j, val );
			feromonas.Set( j, i, val );
		}


	};

}
}
