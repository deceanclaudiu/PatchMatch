#pragma once
class Params
{
public:
	Params(int dispStart = 0, int dispEnd = 255, int winSize = 11, int iterationNo = 4);
	~Params(void);

	const int dispStart;
	const int dispEnd;
	const int winSize;
	const int iterationNo;
};

