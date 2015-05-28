
#pragma once

#include <stdlib.h>
#include <iostream>

template <typename T>
class Matriz
{


private:

	int rows, cols;
	T* data;


public:

	Matriz() : data( NULL )
	{

	}

	Matriz( int rows_, int cols_ ) : data( NULL )
	{
		rows = rows_;
		cols = cols_;
		Alloc( rows, cols );
	}

	Matriz( int size ) : data( NULL )
	{
		rows = size;
		cols = size;
		Alloc( rows, cols );
	}

	Matriz( const Matriz& other ) : data( NULL )
	{
		Alloc( other.rows, other.cols );

		for( int i = 0; i < this->rows; i++ )
		{
			for( int j = 0; j < this->cols; j++ )
			{
				Set( i, j, other.Get( i, j ) );
			}
		}
	}

	~Matriz()
	{
		if( data != NULL )
			delete [] data;
	}

	void Alloc( int rows, int cols )
	{
		if( data != NULL ) delete [] data;
		this->rows = rows;
		this->cols = cols;
		data = new T[rows * cols];
		for( int r = 0; r < rows; r++ )
			for( int c = 0; c < cols; c++ )
				Set( r, c, 0 );
	}

	void Set( int row, int col, T val )
	{
		data[row * this->cols + col] = val;
	}

	void Set( T val )
	{
		for( int i = 0; i < this->rows; i++ )
		{
			for( int j = 0; j < this->cols; j++ )
			{
				Set( i, j, val );
			}
		}
	}

	T Get( int row, int col ) const
	{
		return data[row * this->cols + col];
	}

	void Debug()
	{
		for( int i = 0; i < rows; i++ )
		{
			for( int j = 0; j < cols; j++ )
			{
				std::cout << Get( i, j ) << "\t\t";
			}
			printf("\n");
		}
	}

	void Mayor( int& row, int& col )
	{
		T mayor = Get( 0, 1 );
		row = 0;
		col = 1;
		for( int i = 0; i < rows; i++ )
		{
			for( int j = i+1; j < cols; j++ )
			{
				if( Get( i, j ) > mayor )
				{
					mayor = Get( i, j );
					row = i;
					col = j;
				}
			}
		}
	}

	T SumCol( int col )
	{
		T ret = 0;
		for( int row = 0; row < rows; row++ )
		{
			ret += Get( row, col );
		}
		return ret;
	}

	void SetCol( int col, int val )
	{
		for( int row = 0; row < rows; row++ )
		{
			Set( row, col, val );
		}
	}

	void Inc( int row, int col )
	{
		Set( row, col, Get( row, col ) + 1 );
	}

	void Dec( int row, int col )
	{
		Set( row, col, Get( row, col ) - 1 );
	}


	void Menor( int& row, int& col )
	{
		T menor = Get( 0, 1 );
		row = 0;
		col = 1;
		for( int i = 0; i < rows; i++ )
		{
			for( int j = i+1; j < cols; j++ )
			{
				if( Get( i, j ) < menor )
				{
					menor = Get( i, j );
					row = i;
					col = j;
				}
			}
		}
	}

	int Rows() { return rows; }
	int Cols() { return cols; }



};

template class Matriz<int>;
