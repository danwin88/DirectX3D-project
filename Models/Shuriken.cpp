#include "Shuriken.h"


Shuriken::Shuriken()
{
	isThrown = false;

}

void Shuriken::update(float dt)
{
	if (isThrown == true)
	{
		if (direction == XM_PIDIV2)
		{
			Move(speed*dt);
		}

		else
		{
			Move(-speed*dt);
		}


	}

}








Shuriken::~Shuriken()
{
}

