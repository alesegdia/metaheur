

int toGray( int n )
{
	return n ^ ( n >> 1 );
}


int binHammingDistance( int a, int b )
{
	int gA = toGray( a );
	int gB = toGray( b );
}
