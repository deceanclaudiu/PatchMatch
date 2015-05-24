#pragma once
class Params
{
public:
	Params(int dispStart = 0, int dispEnd = 255, int winSize = 5, int iterationNo = 4, float leftRightConfCheckTh = 0.5);
	~Params(void);

	const int dispStart;
	const int dispEnd;
	const int winSize;
	const int iterationNo;
	const float leftRightConfCheckTh;
};

enum Direction{
	LEFT, 
	RIGHT
};