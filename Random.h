#include <algorithm>
#include <cstdlib>
#include <ctime>

class Random
{
public:
 	Random();
	unsigned int operator()(unsigned int max);
};
	 													
