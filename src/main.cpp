
#include "algoritmo/trayectoria/busquedaaleatoria.h"
#include "algoritmo/trayectoria/busquedalocal.h"
#include "algoritmo/trayectoria/busquedatabu.h"
#include "algoritmo/trayectoria/enfriamientosimulado.h"

#include "algoritmo/genetico/basico.h"
#include "algoritmo/genetico/chc.h"
#include "algoritmo/genetico/multimodal.h"

#include "algoritmo/test.h"
#include "problema/qap.h"

int main( int argc, char** argv )
{

	RNG rng;
	const int NUM_TESTS = 10;

	std::vector<uint32_t> seeds;
	seeds.push_back( 0x01234567 );
	seeds.push_back( 0xFACEFEED );
	seeds.push_back( 0x666DEAD0 );
	seeds.push_back( 0xDEADC0DE );

	/* PROBLEMAS ******************************************/
	QAP<25> tai25;
	tai25.LoadFromFile( "data/tai25b.dat" );
	QAP<90> sko90;
	sko90.LoadFromFile( "data/sko90.dat" );
	QAP<150> tai150;
	tai150.LoadFromFile( "data/tai150b.dat" );
	rng.seed( 0xFACEFEED );


	printf("GREEDY\n");
	printf("=======\n");
	printf("Tai25\n");
	tai25.Greedy().Debug();
	printf("Sko90\n");
	sko90.Greedy().Debug();
	printf("Tai150\n");
	tai150.Greedy().Debug();

	printf("\n\n");


	MH::Trayectoria::BusquedaTabu<QAP<25>> BTTai25 ( tai25, rng );
	test( tai25, BTTai25, NUM_TESTS, true );

	MH::Trayectoria::BusquedaTabu<QAP<90>> BTSko90 ( sko90, rng );
	test( sko90, BTSko90, NUM_TESTS, true );

	MH::Trayectoria::BusquedaTabu<QAP<150>> BTTai150 ( tai150, rng );
	test( tai150, BTTai150, NUM_TESTS, true );

	/* BUSQUEDA ALEATORIA *******************************
	printf("BUSQUEDA ALEATORIA\n");
	printf("==================\n");
	MH::Trayectoria::BusquedaAleatoria<QAP<25>> BATai25 (
			tai25,
			rng,
			1600
		);
	MH::Trayectoria::BusquedaAleatoria<QAP<90>> BASko90 (
			sko90,
			rng,
			1600
		);
	MH::Trayectoria::BusquedaAleatoria<QAP<150>> BATai150 (
			tai150,
			rng,
			1600
		);
	printf("Tai25\n");
	test( tai25, BATai25, NUM_TESTS );
	printf("Sko90\n");
	test( sko90, BASko90, NUM_TESTS );
	printf("Tai150\n");
	test( tai150, BATai150, NUM_TESTS );
	printf("\n\n");*/

	/* BUSQUEDA LOCAL *******************************
	printf("BUSQUEDA LOCAL\n");
	printf("==================\n");
	MH::Trayectoria::BusquedaLocal<QAP<25>> BLTai25 (
			tai25,
			rng
		);
	MH::Trayectoria::BusquedaLocal<QAP<90>> BLSko90 (
			sko90,
			rng
		);
	MH::Trayectoria::BusquedaLocal<QAP<150>> BLTai150 (
			tai150,
			rng
		);
	printf("Tai25\n");
	test( tai25, BLTai25, NUM_TESTS );
	printf("Sko90\n");
	test( sko90, BLSko90, NUM_TESTS );
	printf("Tai150\n");
	test( tai150, BLTai150, NUM_TESTS );
	printf("\n\n");*/

	/* ENFRIAMIENTO SIMULADO ************************
	printf("ENFRIAMIENTO SIMULADO\n");
	printf("=====================\n");
	MH::Trayectoria::EnfriamientoSimulado<QAP<25>> ESTai25 (
			tai25,
			rng,
			0.5f, 	// tanto por uno peor que se acepta al principio
			0.4f, 	// probabilidad de aceptar solucion como arriba
			70,		// vecinos maximos generados
			0.2f 	// porcentaje de maximos para aceptados
		);
	MH::Trayectoria::EnfriamientoSimulado<QAP<90>> ESSko90 (
			sko90,
			rng,
			0.5f, 	// tanto por uno peor que se acepta al principio
			0.4f, 	// probabilidad de aceptar solucion como arriba
			70,		// vecinos maximos generados
			0.2f 	// porcentaje de maximos para aceptados
		);
	MH::Trayectoria::EnfriamientoSimulado<QAP<150>> ESTai150 (
			tai150,
			rng,
			0.5f, 	// tanto por uno peor que se acepta al principio
			0.4f, 	// probabilidad de aceptar solucion como arriba
			70,		// vecinos maximos generados
			0.2f 	// porcentaje de maximos para aceptados
		);

	printf("Tai25\n");
	test( tai25, ESTai25, NUM_TESTS );
	printf("Sko90\n");
	test( sko90, ESSko90, NUM_TESTS );
	printf("Tai150\n");
	test( tai150, ESTai150, NUM_TESTS );
	printf("\n\n");*/


	/* GENETICO BASICO *******************************
	printf("GENETICO BASICO\n");
	printf("==================\n");
	MH::Genetico::Basico<QAP<25>> GBTai25 (
			tai25,
			rng,
			10000,		// iteraciones sin mejorar para salir
			50,			// tamaño de la poblacion
			5,			// torneo N
			0.85f, 		// probabilidad de cruce
			0.15f 		// probabilidad de mutacion
		);
	MH::Genetico::Basico<QAP<90>> GBSko90 (
			sko90,
			rng,
			10000,		// iteraciones sin mejorar para salir
			50,			// tamaño de la poblacion
			5,			// torneo N
			0.85f, 		// probabilidad de cruce
			0.15f 		// probabilidad de mutacion
		);
	MH::Genetico::Basico<QAP<150>> GBTai150 (
			tai150,
			rng,
			10000,		// iteraciones sin mejorar para salir
			50,			// tamaño de la poblacion
			5,			// torneo N
			0.85f, 		// probabilidad de cruce
			0.15f 		// probabilidad de mutacion
		);
	printf("Tai25\n");
	test( tai25, GBTai25, seeds, true );
	printf("Sko90\n");
	test( sko90, GBSko90, seeds, true );
	printf("Tai150\n");
	test( tai150, GBTai150, seeds, true );
	printf("\n\n");*/





	/* GENETICO CHC ***************************
	printf("GENETICO CHC\n");
	printf("==================\n");
	MH::Genetico::CHC<QAP<25>> CHTai25 (
			tai25,
			rng,
			50,		// cantidad de poblacion
			20		// numero de reinicios
		);
	MH::Genetico::CHC<QAP<90>> CHSko90 (
			sko90,
			rng,
			50,		// cantidad de poblacion
			20		// numero de reinicios
		);
	MH::Genetico::CHC<QAP<150>> CHTai150 (
			tai150,
			rng,
			50,		// cantidad de poblacion
			20		// numero de reinicios
		);
	printf("Tai25\n");
	test( tai25, CHTai25, seeds, true );
	printf("Sko90\n");
	test( sko90, CHSko90, seeds, true );
	printf("Tai150\n");
	test( tai150, CHTai150, seeds, true );
	printf("\n\n");*/



	/* GENETICO MULTIMODAL ********************
	printf("GENETICO MULTIMODAL\n");
	printf("==================\n");
	MH::Genetico::Multimodal<QAP<25>> MMTai25 (
			tai25,
			rng,
			5000,	// iteraciones sin mejorar
			80,		// maximo de la poblacion
			3,		// ganadores en torneo
			5,		// ganadores por nicho
			45.f,		// sigma
			0.8, 	// probabilidad de cruce
			0.01	// probabilidad de mutacion 
		);
	MH::Genetico::Multimodal<QAP<90>> MMSko90 (
			sko90,
			rng,
			10000,	// iteraciones sin mejorar
			80,		// maximo de la poblacion
			3,		// ganadores en torneo
			5,		// ganadores por nicho
			45.f,		// sigma
			0.8, 	// probabilidad de cruce
			0.01	// probabilidad de mutacion 
		);
	MH::Genetico::Multimodal<QAP<150>> MMTai150 (
			tai150,
			rng,
			10000,	// iteraciones sin mejorar
			80,		// maximo de la poblacion
			3,		// ganadores en torneo
			5,		// ganadores por nicho
			45.f,		// sigma
			0.8, 	// probabilidad de cruce
			0.01	// probabilidad de mutacion 
		);

	printf("Tai25\n");
	test( tai25, MMTai25, seeds, true );
	printf("Sko90\n");
	test( sko90, MMSko90, seeds, true );
	printf("Tai150\n");
	test( tai150, MMTai150, seeds, true );
	printf("\n\n");*/


	return 0;
}
