#pragma once
const int g_maxDispCnt = 260;
const int g_maxWidth = 1248;
const int g_maxHeight = 376;
class Params
{
public:
	Params(int dispStart = 0, int dispEnd = 255, int winSize = 5, int iterationNo = 4, float leftRightConfCheckTh = 1.0);
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