#include "Random.h"

Random::Random()
{
	srand( static_cast<unsigned int>( time(NULL) ) );
}
// 関数オブジェクト
unsigned int Random::operator()(unsigned int max)
{
	double tmp = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
	return static_cast<unsigned int>( tmp * max );
}

