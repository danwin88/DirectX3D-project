#include "GameObject.h"
#include "Texture.h"

GameObject::GameObject()
{
	Size = 1;
	spin = 0;
}

//Getters
XMVECTOR GameObject::getPosition()
{
	return Position;

}

float GameObject::getSize()
{
	return Size;
}


XMMATRIX* GameObject::getWorldmat()
{
	return &worldMat;

}

Texture* GameObject::getTexture()
{
	return TextureData;
}

Mesh* GameObject::getMesh()
{
	return MeshData;
}

//Setters

void GameObject::setTexture(Texture* texture)
{
	TextureData = texture;
}


void GameObject::setMesh(Mesh* mesh)
{
	MeshData = mesh;
}


void GameObject::setPosition(static float x, static float y, static float z)
{
	Position = XMVectorSet(x, y, z, 1.0f);

	updateWorldmat();

}

void GameObject::setPosition(XMVECTOR vec)
{
	Position = vec;
}


void GameObject::setSize(float s)
{
	Size = s;

	updateWorldmat();
}

//void GameObject::setSize(float x, float y, float z)
//{
//	Size = x, y, z;
//
//	updateWorldmat();
//}

void GameObject::updateWorldmat()
{
	XMMATRIX rotate = XMMatrixRotationY(direction);
	XMMATRIX rotatez = XMMatrixRotationZ(spin);
	XMMATRIX scale = XMMatrixScaling(Size, Size, Size);
	XMMATRIX translate = XMMatrixTranslation(XMVectorGetX(Position), XMVectorGetY(Position), XMVectorGetZ(Position));
	worldMat = scale * rotate * rotatez * translate;
	worldMat = XMMatrixTranspose(worldMat);

}

bool GameObject::isFacingRight()
{
	if (direction == XM_PIDIV2)
	{
		return true;
	}

	else
	{
		return false;
	}
	

}

void GameObject::setSpin(float s)
{
	spin += s;
}

void GameObject::resetSpin()
{
	spin = 0;
}



void GameObject::faceleft()
{
	direction = -XM_PIDIV2;

	updateWorldmat();

}


void GameObject::faceright()
{
	direction = XM_PIDIV2;

	updateWorldmat();
}

GameObject::~GameObject()
{
}
