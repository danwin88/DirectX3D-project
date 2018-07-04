#include "Texture.h"


bool Texture::LoadContent(ID3D11Device* d3dDevice_, char* filename)
{
	HRESULT d3dResult = D3DX11CreateShaderResourceViewFromFile
	(d3dDevice_,filename, 0, 0, &texturedata, 0);


	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to load the texture image!");
		return false;
	}

	return this;
}


void Texture::UnloadContent()
{
	if (texturedata) texturedata->Release();

	texturedata = 0;

}


ID3D11ShaderResourceView* const* Texture::Render()
{

	return &texturedata;
}