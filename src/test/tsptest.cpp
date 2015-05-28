
#include "../algoritmo/hormigas/sh.h"
#include "../algoritmo/hormigas/she.h"
#include "../algoritmo/trayectoria/busquedalocal.h"
#include "../algoritmo/test.h"
#include "../problema/qap.h"
#include "../problema/tsp.h"
#include "../algoritmo/hormigas/sch.h"

int main( int argc, char** argv )
{

	RNG rng;

	std::vector<uint32_t> seeds;
	seeds.push_back( 0x01234567 );
	seeds.push_back( 0xFACEFEED );
	seeds.push_back( 0x666DEAD0 );
	seeds.push_back( 0xDEADC0DE );

	TSP<130> ch130;
	ch130.LoadFromFile( "data/ch130.tsp" );
	printf("===GREEDY===\n");
	ch130.Greedy().Debug();
	printf("************\n");

	TSP<279> a279;
	a279.LoadFromFile( "data/a280.tsp" );
	printf("===GREEDY===\n");
	a279.Greedy().Debug();
	printf("************\n");

	TSP<654> p654;
	p654.LoadFromFile( "data/p654.tsp" );
	printf("===GREEDY===\n");
	p654.Greedy().Debug();
	printf("************\n");

	rng.seed( 0xFACEFEED );

	MH::Hormigas::SH<TSP<130>> TSP_sh130(
			ch130,
			rng,
			5,		// minutos de funcionamiento
			20,		// numero de hormigas
			1,		// exponente de feromonas
			2,		// exponente de heuristica
			0.1		// factor de evaporación
	);


	MH::Hormigas::SH<TSP<279>> TSP_sh279(
			a279,
			rng,
			10,		// minutos de funcionamiento
			20,		// numero de hormigas
			1,		// exponente de feromonas
			2,		// exponente de heuristica
			0.1		// factor de evaporación
	);
	MH::Hormigas::SH<TSP<654>> TSP_sh654(
			p654,
			rng,
			15,		// minutos de funcionamiento
			20,		// numero de hormigas
			1,		// exponente de feromonas
			2,		// exponente de heuristica
			0.1		// factor de evaporación
	);


	MH::Hormigas::SHE<TSP<130>> TSP_she130(
			ch130,
			rng,
			5,		// minutos de funcionamiento
			20,		// numero de hormigas
			1,		// exponente de feromonas
			2,		// exponente de heuristica
			0.1		// factor de evaporación
	);
	MH::Hormigas::SHE<TSP<279>> TSP_she279(
			a279,
			rng,
			10,		// minutos de funcionamiento
			20,		// numero de hormigas
			1,		// exponente de feromonas
			2,		// exponente de heuristica
			0.1		// factor de evaporación
	);
	MH::Hormigas::SHE<TSP<654>> TSP_she654(
			p654,
			rng,
			15,		// minutos de funcionamiento
			20,		// numero de hormigas
			1,		// exponente de feromonas
			2,		// exponente de heuristica
			0.1		// factor de evaporación
	);



	MH::Hormigas::SCH<TSP<130>> TSP_sch130(
			ch130,
			rng,
			5,		// minutos de funcionamiento
			20,		// numero de hormigas
			1,		// exponente de feromonas
			2,		// exponente de heuristica
			0.1f,		// factor de evaporación
			0.1f,
			0.98f
	);
	MH::Hormigas::SCH<TSP<279>> TSP_sch279(
			a279,
			rng,
			10,		// minutos de funcionamiento
			20,		// numero de hormigas
			1,		// exponente de feromonas
			2,		// exponente de heuristica
			0.1f,		// factor de evaporación
			0.1f,
			0.98f
	);
	MH::Hormigas::SCH<TSP<654>> TSP_sch654(
			p654,
			rng,
			15,		// minutos de funcionamiento
			20,		// numero de hormigas
			1,		// exponente de feromonas
			2,		// exponente de heuristica
			0.1f,		// factor de evaporación
			0.1f,
			0.98f
	);


	printf("HORMIGAS BASICO\n==================\n");
	test( ch130, TSP_sh130,  5, true );
	test( a279,  TSP_sh279,  5, true );
	test( p654,  TSP_sh654,  5, true );

	printf("HORMIGAS ELITE\n==================\n");
	test( ch130, TSP_she130, 5, true );
	test( a279,  TSP_she279, 5, true );
	test( p654,  TSP_she654, 5, true );

	printf("HORMIGAS COLONIA\n==================\n");
	test( ch130, TSP_sch130, 5, true );
	test( a279,  TSP_sch279, 5, true );
	test( p654,  TSP_sch654, 5, true );

	return 0;
}
