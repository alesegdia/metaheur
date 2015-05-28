

#pragma once

#include <vector>

template <typename Problema>
class RepresentacionBase
{


public:

	class Solucion
	{

	public:


		Solucion()
		{

		}

		virtual ~Solucion()
		{

		}

		virtual void GenerarVecinos( std::vector<Solucion>& vec )
		{
			printf("SIN IMPLEMENTAR\n");

		}

		virtual void Mutar( RNG& rng )
		{
			printf("SIN IMPLEMENTAR\n");
		}

		virtual void Randomizar( RNG& rng, int iteraciones )
		{
			printf("SIN IMPLEMENTAR\n");

		}

		virtual void Cruzar( const Solucion& sol1, const Solucion& sol2, RNG& rng )
		{
			printf("SIN IMPLEMENTAR\n");

		}

		void Debug() const
		{
			printf("SIN IMPLEMENTAR\n");

		}

		void CalcularFitness( Problema& inst )
		{
			this->fitness = inst.Fitness( (*(dynamic_cast<typename Problema::Solucion*>(this))) );
		}

		float GetFitness( ) const
		{
			return fitness;
		}

		void SetFitness( float f )
		{
			this->fitness = f;
		}

		bool operator< (const Solucion& sol ) const
		{
			return fitness < sol.fitness;
		}

		virtual int DistanciaHamming( const Solucion& other )
		{
			return 0;
		}


	protected:

		float fitness;

	};


	RepresentacionBase()
	{

	}

	virtual ~RepresentacionBase()
	{

	}

	virtual float Fitness( const Solucion& sol )
	{
		printf("SIN IMPLEMENTAR\n");

		return -1;
	}


};
