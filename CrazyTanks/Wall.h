#pragma once

#include "Structures.h"

class Wall
{
public:
	Wall() {}

	void setPointStartX(unsigned _x) { pStart.x = _x; }
	void setPointStartY(unsigned _y) { pStart.y = _y; }
	void setPointEndX(unsigned _x) { pEnd.x = _x; }
	void setPointEndY(unsigned _y) { pEnd.y = _y; }

	Point getPointStart() { return pStart; }
	Point getPointEnd() { return pEnd; }

private:
	Point pStart;
	Point pEnd;

};
