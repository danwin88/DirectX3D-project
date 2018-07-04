#pragma once
#include "MovingObject.h"
#include "Mesh.h";

class Shuriken: public MovingObject
{
public: 
	Shuriken();
	~Shuriken();

	bool isThrown;

	void update(float dt);



};

