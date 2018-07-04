#include "MovingObject.h"



MovingObject::MovingObject()
{
	GameObject();
	jumping = false;
	maxJump = 4.0;
	jumpSpeed = 10.0;
	falling = false;
	crouching = false;
	speed = 3.0;
	health = 0;

}


void MovingObject::Move(float x)
{	
	float tempx = XMVectorGetX(Position);

	tempx = x + tempx;

	Position = XMVectorSetX(Position, tempx);

	updateWorldmat();
}

void MovingObject::MoveY(float y)
{
	float tempy = XMVectorGetY(Position);

	tempy += y;

	Position = XMVectorSetY(Position, tempy);

	updateWorldmat();
}


int MovingObject::getHealth()
{
	return health;
}

void MovingObject::SetHealth(float hp)
{
	health = hp;
}



void MovingObject::jump()
{

	if (jumping == false && falling == false)
	{
		jumping = true;
	}


}

void MovingObject::crouch()
{
	if (crouching == false && jumping == false && falling == false)
	{
		crouching = true;
	}

}

void MovingObject::stopcrouch()
{
	crouching = false;
}

bool MovingObject::isDead()
{
	if (health <= 0)
	{
		return true;
	}

	else
	{
		false;
	}

}

bool MovingObject::isJumping()
{
	return jumping;

}

bool MovingObject::isCrouching()
{
	return crouching;

}


void MovingObject::setSpeed(float s)
{
	speed = s;
}

void MovingObject::getDamage(float damage)
{
	health -= damage;
}

float MovingObject::getSpeed()
{
	return speed;
}


void MovingObject::update(float dt)
{
	if (jumping == true)
	{
		if (falling == false)
		{
			float tempy = XMVectorGetY(Position);
			tempy = jumpSpeed*dt + tempy;

			Position = XMVectorSetY(Position, tempy);
		}
	}

	if (XMVectorGetY(Position) >= maxJump)
	{
		jumping = false;
		falling = true;


	}

	if (falling == true)
	{

		float tempy = XMVectorGetY(Position);
		tempy = tempy - jumpSpeed*dt;

		if (tempy < 0)
		{
			falling = false;
			tempy = 0;
		}

		Position = XMVectorSetY(Position, tempy);



	}

	updateWorldmat();

}

void MovingObject::SetJumpspeed(float js)
{
	jumpSpeed = js;
}

void MovingObject::SetMaxJump(float ms)
{
	maxJump = ms;
}






MovingObject::~MovingObject()
{
}


