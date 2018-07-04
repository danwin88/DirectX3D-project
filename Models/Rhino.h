#pragma once
#include "MovingObject.h"

class Rhino : public MovingObject
{
public:
	Rhino();
	~Rhino();

	float rhinoturntime;
	bool rhinoturn = 0.0f;
	bool previosturn = true;
	bool currentturn = true;



};

