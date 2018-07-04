#pragma once
#include "Texture.h"
#include "Mesh.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	//Getters
	XMVECTOR getPosition();
	float getSize();
	XMMATRIX* getWorldmat();


	Texture* getTexture();
	Mesh* getMesh();

	//Setters
	void setPosition(static float x, static float y, static float z);
	void setPosition(XMVECTOR);
	void setSize(float);
	void setSize(float, float, float);

	bool isFacingRight();

	void setSpin(float);
	void resetSpin();

	void setTexture(Texture* texture);
	void setMesh(Mesh* mesh);
	
	void faceleft();
	void faceright();
	

	void updateWorldmat();


protected:

	//Data
	Texture* TextureData;
	Mesh* MeshData;

	float direction;
	float spin;

	XMVECTOR Position;
	float Size;
	XMMATRIX worldMat;


};