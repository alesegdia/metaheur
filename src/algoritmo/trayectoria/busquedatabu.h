

#pragma once


#include <limits>
#include <stdio.h>
#include <vector>
#include "../../util/random.h"
#include "../../util/matriz.h"
#include "../../problema/representacion/base/base.h"
#include "../metodo.h"



namespace MH
{
namespace Trayectoria
{

	template <typename Problema>
	class BusquedaTabu : public MH::Metodo<Problema>
	{

		typedef typename Problema::Solucion Solucion;
		typedef typename Problema::Poblacion Poblacion;

		struct MovimientoTabu {

			int emplazamiento[2], unidad[2];

			void Debug()
			{
				printf("e0: %d, u0: %d\n", emplazamiento[0], unidad[0]);
				printf("e1: %d, u1: %d\n", emplazamiento[1], unidad[1]);
			}

		};


		class MemoriaTabu {

		private:

			int tenencia_tabu;

			Matriz<int> stmem;
			Matriz<int> ltmem;

			void Reset( Matriz<int>& mem )
			{
				for( int i = 0; i < Problema::PROB_SIZE; i++ )
				{
					mem.SetCol( i, 0 );
				}
			}

		public:

			MemoriaTabu() :
				stmem( Problema::PROB_SIZE ),
				ltmem( Problema::PROB_SIZE )
			{
				Reset( stmem );
				Reset( ltmem );
			}
			~MemoriaTabu() {}

			bool EsTabuActivo( MovimientoTabu mt )
			{
				// si alguno de los movimientos es tabu activo
				return stmem.Get( mt.emplazamiento[0], mt.unidad[0] ) > 0 ||
					   stmem.Get( mt.emplazamiento[1], mt.unidad[1] ) > 0;
			}

			void Actualizar( MovimientoTabu mov )
			{
				// actualizamos memoria a largo plazo
				ltmem.Inc( mov.emplazamiento[0], mov.unidad[0] );
				ltmem.Inc( mov.emplazamiento[1], mov.unidad[1] );

				// actualizamos memoria a corto plazo
				for( int i = 0; i < Problema::PROB_SIZE; i++ )
				{
					for( int j = 0; j < Problema::PROB_SIZE; j++ )
					{
						int el = stmem.Get(i,j);
						stmem.Set( i, j, (el <= 0 ? el : el-1) );
					}
				}
				stmem.Set( mov.emplazamiento[0], mov.unidad[0], tenencia_tabu );
				stmem.Set( mov.emplazamiento[1], mov.unidad[1], tenencia_tabu );
			}

			void BorrarCortoPlazo()
			{
				Reset( stmem );
			}

			void ActualizarTenencia( int newsize )
			{
				tenencia_tabu = newsize;
				if( tenencia_tabu < 1 ) tenencia_tabu = 1;
			}

			Solucion GenerarGreedy( RNG& rng )
			{
				int unidades[Problema::PROB_SIZE];

				for( int i = 0; i < Problema::PROB_SIZE; i++ )
				{
					unidades[i] = i;
				}

				for( int emplazamiento = 0; emplazamiento < Problema::PROB_SIZE; emplazamiento++ )
				{
					// RULETA PROPORCIONAL PARA ELEGIR LA ENTIDAD QUE VA A ENTRAR EN EL EMPLAZAMIENTO
					int unidad_elegida = Problema::PROB_SIZE-1;
					float total_aportacion = 0.f;
					float aportaciones[Problema::PROB_SIZE] = { 0 };

					// recorremos las unidades para calcular las aportaciones
					for( int j = emplazamiento; j < Problema::PROB_SIZE; j++ )
					{
						aportaciones[j] = 1 / ( 1 + ((float)ltmem.Get(emplazamiento,unidades[j])) );
						total_aportacion += aportaciones[j];
					}

					// comprobamos que unidad toca a partir del uniform generado
					float unif = rng.uniform();
					float current = aportaciones[emplazamiento] / total_aportacion;
					for( int j = emplazamiento; j < Problema::PROB_SIZE; j++ )
					{
						if( unif < current )
						{
							unidad_elegida = unidades[j];
							break;
						}
						aportaciones[j] /= total_aportacion;
						current += aportaciones[j];
					}

					// colocamos la entidad en el emplazamiento que le corresponde
					std::swap( unidades[emplazamiento], unidades[unidad_elegida] );


				}

				return Solucion( unidades );
			}

			void Debug()
			{
				printf("CORTO PLAZO:\n");
				printf("============\n");
				stmem.Debug();
				printf("LARGO PLAZO:\n");
				printf("============\n");
				ltmem.Debug();
				printf("\n\n\n");
			}

		};


	public:

		BusquedaTabu(
				Problema& pinst,
				RNG& prng )
			: MH::Metodo<Problema>( pinst, prng ){}

		Solucion operator()()
		{
			Solucion actual, mejor_global;
			MemoriaTabu tabumem;
			int num_iteraciones, iteracion_reset, tenencia_tabu_actual;

			// 40 vecinos
			// RESET:
			// 	* 0.25 random
			// 	* 0.25 best
			// 	* 0.50 largo plazo

			actual.ValidReset();
			actual.Randomizar( this->rng );
			actual.CalcularFitness( this->inst );

			mejor_global = actual;

			num_iteraciones = 40 * Problema::PROB_SIZE;
			iteracion_reset = 8 * Problema::PROB_SIZE;
			tenencia_tabu_actual = 2;
			tabumem.ActualizarTenencia( tenencia_tabu_actual );

			while( --num_iteraciones > 0 )
			{
				MovimientoTabu mov_actual;
				Solucion mejor_vecino;
				mejor_vecino.SetFitness(std::numeric_limits<float>::max());

				for( int i = 0; i < 40; i++ )
				{
					Solucion tmp;
					MovimientoTabu mov;

					// GENERAR VECINO Y CAPTURAR MOVIMIENTO
					tmp = actual;
					tmp.Mutar( this->rng,
							mov.emplazamiento[0], mov.unidad[0],
							mov.emplazamiento[1], mov.unidad[1] );
					tmp.CalcularFitness( this->inst );

					// ACEPTAMOS SI ES TABU O CUMPLE ASPIRACIÓN
					if( !tabumem.EsTabuActivo( mov ) ||
						tmp.GetFitness() < mejor_global.GetFitness() )
					{
						// SUSTITUIMOS SI ES MEJOR QUE EL VECINO MEJOR ACTUAL
						if( tmp.GetFitness() < mejor_vecino.GetFitness() )
						{
							mejor_vecino = tmp;
							mov_actual = mov;
						}
					}
				}

				// ACTUALIZAR MEMORIAS
				tabumem.Actualizar( mov_actual );

				// ANOTAMOS SI ES MEJOR QUE LA GLOBAL
				if( mejor_vecino.GetFitness() < mejor_global.GetFitness() )
				{
					mejor_global = mejor_vecino;
				}

				actual = mejor_vecino;


				// SI TOCA REARRANQUE
				if( num_iteraciones % iteracion_reset == 0 )
				{
					float p = this->rng.uniform();

						// ALEATORIO
					if( p < 0.25 )
					{
						actual.ValidReset();
						actual.Randomizar( this->rng );
						actual.CalcularFitness( this->inst );
					}
						// MEJOR GLOBAL
					else if( p < 0.5 )
					{
						actual = mejor_global;
					}
						// GREEDY
					else
					{
						actual = tabumem.GenerarGreedy( this->rng );
						actual.CalcularFitness( this->inst );
						//actual.Debug();
					}

					// ACTUALIZAR TENENCIA
					tenencia_tabu_actual *= (this->rng.uniform() > 0.5) ? 2 : 0.5;
					tabumem.ActualizarTenencia( tenencia_tabu_actual );

					// RESETEAR CORTO PLAZO
					tabumem.BorrarCortoPlazo();
				}
			}

			return mejor_global;
		}
	};

}
}
