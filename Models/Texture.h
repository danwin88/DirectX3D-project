#pragma once

#ifndef TEXTURE_H_
#define TEXTURE_H_


#include"Dx11DemoBase.h"
#include <xnamath.h>
#include <stdio.h>
#include "objLoader.h"

class Texture
{
public:


	bool LoadContent(ID3D11Device* d3dDevice_, char* );
	void UnloadContent();
	ID3D11ShaderResourceView* const* Render();



private:


	ID3D11ShaderResourceView* texturedata;

};

#endif