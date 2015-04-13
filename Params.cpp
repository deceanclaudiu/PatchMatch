#include "Params.h"

Params::Params(int dispStart, int dispEnd, int winSize, int iterationNo, float leftRightConfCheckTh):
	dispStart(dispStart),
	dispEnd(dispEnd),
	winSize(winSize),
	iterationNo(iterationNo),
	leftRightConfCheckTh(leftRightConfCheckTh)
{
}

Params::~Params(void)
{
}
