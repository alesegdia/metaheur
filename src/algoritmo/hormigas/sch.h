

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
	class SCH: public MH::Metodo<Problema>
	{

		typedef typename Problema::Solucion Solucion;
		typedef typename Problema::Poblacion Poblacion;

		Matriz<float> feromonas;
		Solucion solgreedy;
		float feromona_inicial, alfa, beta, rho, phi, q0;
		int minutos_limite, num_hormigas;

	public:

		SCH(
				Problema& pinst,
				RNG& prng,
				int min_lim,
				int num_hor,
				float alfa_,
				float beta_,
				float rho_,
				float phi_,
				float q0_) :
			MH::Metodo<Problema>( pinst, prng ),
			feromonas( Problema::PROB_SIZE )
		{
			solgreedy = pinst.Greedy();
			feromona_inicial = 1.f / (((float)Problema::PROB_SIZE) * solgreedy.GetFitness());
			minutos_limite = min_lim;
			num_hormigas = num_hor;
			alfa = alfa_;
			beta = beta_;
			rho = rho_;
			phi = phi_;
			q0 = q0_;
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
					SetFeromonas( i, j, feromona_inicial );
				}
			}

			Timer t;
			t.Start();
			while( t.Get() < minutos_limite * 60 )
			{
				for( int i = 0; i < num_hormigas; i++ )
				{
					Solucion actual = CalcularCaminoHormiga();
					ActualizacionLocal( actual );
					if( actual.GetFitness() < mejor_global.GetFitness() )
					{
						//printf("%d %d\n", (int)(round(actual.GetFitness())), t.Get() );
						fflush(0);
						mejor_global = actual;
					}
				}
				ActualizacionGlobal( mejor_global );
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
				float total_aportacion = 0.f;
				float probabilidades[Problema::PROB_SIZE] = { 0 };

				// CALCULAMOS LOS NUMERADORES Y EL SUMATORIO DE LAS PROBABILIDADES
				for( int j = i; j < Problema::PROB_SIZE; j++ )
				{
					float fero, heur;
					fero = GetFeromonas( sol[i-1], sol[j] );
					heur = this->inst.GetHeuristica( sol[i-1], sol[j] );

					probabilidades[j] = pow( fero, alfa ) * pow( heur, beta );
					total_aportacion += probabilidades[j];
				}

				int ciudad_elegida = Problema::PROB_SIZE - 1;
				float q = this->rng.uniform();
				if( q <= q0 )
				{
					// EXPLOTACION
					for( int j = i; j < Problema::PROB_SIZE-1; j++ )
					{
						if( probabilidades[j] > probabilidades[ciudad_elegida] )
						{
							ciudad_elegida = j;
						}
					}
				}
				else
				{
					// EXPLORACION DIRIGIDA
					float current = probabilidades[i] / total_aportacion;
					float unif = this->rng.uniform();
					for( int j = i; j < Problema::PROB_SIZE-1; j++ )
					{
						if( unif < current )
						{
							ciudad_elegida = j;
							break;
						}
						probabilidades[j+1] /= total_aportacion;
						current += probabilidades[j+1];
					}
				}

				// INTERCAMBIAMOS LAS POSICIONES
				std::swap( sol[i], sol[ciudad_elegida] );
			}

			Solucion ret( sol );
			ret.CalcularFitness( this->inst );
			return ret;
		}

		// ACTUALIZACIÓN GLOBAL ****************************************
		float CalcularAporteGlobal( float feromonas, float fitness )
		{
			return feromonas * ( 1.f - rho ) + rho / fitness;
		}

		void ActualizacionGlobal( const Solucion& sol )
		{
			// APORTE: aporta solo el camino
			for( int i = 1; i < Problema::PROB_SIZE; i++ )
			{
				int f1, f2;
				f1 = sol.Get(i);
				f2 = sol.Get(i-1);
				float oldfer = GetFeromonas( f1, f2 );
				SetFeromonas( f1, f2, CalcularAporteGlobal( oldfer, sol.GetFitness() ) );
			}

			float oldfer = GetFeromonas( sol.Get(0), sol.Get(Problema::PROB_SIZE-1) );
			SetFeromonas( sol.Get(0), sol.Get( Problema::PROB_SIZE - 1 ), CalcularAporteGlobal( oldfer, sol.GetFitness() ) );
		}


		// ACTUALIZACIÓN LOCAL *****************************************
		float CalcularAporteLocal( float feromonas )
		{
			return feromonas * ( 1.f - phi ) + feromona_inicial * phi;
		}

		void ActualizacionLocal( const Solucion& sol )
		{
			for( int i = 1; i < Problema::PROB_SIZE; i++ )
			{
				int f1, f2;
				f1 = sol.Get(i);
				f2 = sol.Get(i-1);
				float oldfer = GetFeromonas( f1, f2 );
				SetFeromonas( f1, f2, CalcularAporteLocal( oldfer ) );
			}

			float oldfer = GetFeromonas( sol.Get(0), sol.Get(Problema::PROB_SIZE-1) );
			SetFeromonas( sol.Get(0), sol.Get( Problema::PROB_SIZE - 1 ), CalcularAporteLocal( oldfer ) );

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
