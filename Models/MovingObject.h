#pragma once
#include "GameObject.h"


class MovingObject : public GameObject
{
public:

	
	MovingObject();
	~MovingObject();


	void Move(float);
	void MoveY(float);
	void jump();
	void crouch();
	void stopcrouch();

	//bools
	bool isDead();
	bool isJumping();
	bool isCrouching();

	//getters
	void getDamage(float damage);
	float getSpeed();
	int getHealth();
	
	//setters
	void setSpeed(float);
	void SetJumpspeed(float);
	void SetMaxJump(float);
	void SetHealth(float);


	void update(float dt);


protected:

	int health;
	float speed;
	float damage;

	float jumpSpeed = 2.0;
	float maxJump = 10000.0;

	bool jumping = false;
	bool falling = false;
	bool dead = false;
	bool crouching = false;

};