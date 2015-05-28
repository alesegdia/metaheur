
#pragma once

#include <stdio.h>
#include <math.h>

#include "base.h"

/**
 *	Clase base para las soluciones de tipo array
 * */

template <typename GenType, size_t PROBSIZE>
class ArrayBase
{

protected:

	GenType data[PROBSIZE];

public:

	ArrayBase(){}
	ArrayBase( const ArrayBase& other )
	{
		Copy( other );
	}

	virtual ~ArrayBase()
	{

	}

	void Set( int index, GenType val )
	{
		data[index] = val;
	}

	void Copy( const ArrayBase& other )
	{
		for( int i = 0; i < PROBSIZE; i++ )
		{
			this->data[i] = other.data[i];
		}
	}

	GenType Get( int index ) const
	{
		return data[index];
	}

	void Debug( ) const
	{
		for( int i = 0; i < PROBSIZE; i++ )
		{
			std::cout << data[i] << ", ";
		}
		printf( "\n" );
	}

	void Swap( int i, int j )
	{
		GenType tmp = data[i];
		data[i] = data[j];
		data[j] = tmp;
	}

	void Reset( int val )
	{
		for( int i = 0; i < PROBSIZE; i++ )
		{
			data[i] = val;
		}
	}

	virtual ArrayBase& operator=( const ArrayBase &rhs )
	{
		Copy( rhs );
		return (*this);
	}

	virtual bool operator==( const ArrayBase &rhs )
	{
		for( int i = 0; i < PROBSIZE; i++ )
		{
			if( rhs.data[i] != data[i] )
				return false;
		}
		return true;
	}


};

