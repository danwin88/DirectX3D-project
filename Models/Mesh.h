#pragma once

#ifndef MESH_H_
#define MESH_H_


#include"Dx11DemoBase.h"
#include "Vertexes.h"
#include <xnamath.h>
#include <stdio.h>
#include "objLoader.h"

class Mesh
{
public:

	bool LoadMesh(ID3D11Device* d3dDevice_, char*);
	void UnloadMesh();
	ID3D11Buffer* const* Render();

	int gettotalVertex();


private:


	ID3D11Buffer* meshdata;
	int totalVertex;

};

#endif