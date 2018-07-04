#include "Mesh.h"

bool Mesh::LoadMesh(ID3D11Device* d3dDevice_, char* filename)
{
	// Load the models from the file.
	ObjModel objModel;

	//if( objModel.LoadOBJ( "tank.obj" ) == false )
	//if( objModel.LoadOBJ( "sphere.obj" ) == false )
	//if( objModel.LoadOBJ( "TestBlock.obj" ) == false )
	if (objModel.LoadOBJ(filename) == false)
	{
		DXTRACE_MSG("Error loading 3D model!");
		return false;
	}

	totalVertex = objModel.GetTotalVerts();

	VertexPos* vertices = new VertexPos[totalVertex];
	float* vertsPtr = objModel.GetVertices();
	float* texCPtr = objModel.GetTexCoords();
	float* normalPtr = objModel.GetNormals();

	for (int i = 0; i < totalVertex; i++)
	{
		vertices[i].pos = XMFLOAT3(*(vertsPtr + 0), *(vertsPtr + 1), *(vertsPtr + 2));
		vertsPtr += 3;

		vertices[i].tex0 = XMFLOAT2(*(texCPtr + 0), *(texCPtr + 1));
		texCPtr += 2;

		vertices[i].norm = XMFLOAT3(*(normalPtr + 0), *(normalPtr + 1), *(normalPtr + 2));
		normalPtr += 3;
	}

	D3D11_BUFFER_DESC vertexDesc;

	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * totalVertex;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	HRESULT d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &meshdata);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to create vertex buffer!");
		return false;
	}

	delete[] vertices;
	objModel.Release();
}


int Mesh::gettotalVertex()
{
	return totalVertex;
}

void Mesh::UnloadMesh()
{
	if (meshdata) meshdata->Release();

	meshdata = 0;

}


ID3D11Buffer* const* Mesh::Render()
{
	return &meshdata;

}