/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    3D Models Demo - Demonstrates loading a model from an .OBJ file.
*/


#include"ModelsDemo.h"
#include<xnamath.h>
#include<stdio.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include"objLoader.h"
#include "Texture.h"
#include "Shuriken.h"
#include "Frog.h"
#include "Rhino.h"


char* TEXTURE_NAME = "Concrete_texture.jpg";
char* MODEL_NAME = "tank.obj";

using namespace std;
animation playerstate;





ModelsDemo::ModelsDemo( ) : textureMapVS_( 0 ), textureMapPS_( 0 ),textTextureMapVS_( 0 ), textTextureMapPS_( 0 ),
							inputLayout_( 0 ), textInputLayout_(0),textVertexBuffer_(0),textColorMapSampler_( 0 ),
                            vertexBuffer_( 0 ), colorMap_( 0 ), textColorMap_(0), colorMapSampler_( 0 ),
                            viewCB_( 0 ), projCB_( 0 ), worldCB_( 0 ), camPosCB_( 0 ), totalVerts_( 0 ),
							gameState_(PLAY_INTRO), pauseMenuSelection(RETURN), displayFPS(false)
{
    ZeroMemory( &controller1State_, sizeof( XINPUT_STATE ) );
    ZeroMemory( &prevController1State_, sizeof( XINPUT_STATE ) );
	ZeroMemory(&mouseCurrState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mousePrevState, sizeof(DIMOUSESTATE));

	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;
	m_depthStencilState = 0;
	m_depthDisabledStencilState = 0;
	frameTime_=0;
	fps_=0;
	sCounter = 0;

	for (int i = 0; i < NUM_FROGS; i++)

	{
		frogcurrentframe[i] = 0;
		frogframetime[i] = 0;

	}

	for (int i = 0; i < NUM_RHINOS; i++)

	{
		wrhinocurrentframe[i] = 0;
		wrhinoframetime[i] = 0;

	}




}


ModelsDemo::~ModelsDemo( )
{

}


bool ModelsDemo::LoadContent( )
{

    ID3DBlob* vsBuffer = 0;

    bool compileResult = CompileD3DShader( "TextureMap.fx", "VS_Main", "vs_4_0", &vsBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling the vertex shader!" );
        return false;
    }

    HRESULT d3dResult;

    d3dResult = d3dDevice_->CreateVertexShader( vsBuffer->GetBufferPointer( ),
        vsBuffer->GetBufferSize( ), 0, &textureMapVS_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the vertex shader!" );

        if( vsBuffer )
            vsBuffer->Release( );

        return false;
    }

    D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );

    d3dResult = d3dDevice_->CreateInputLayout( solidColorLayout, totalLayoutElements,
        vsBuffer->GetBufferPointer( ), vsBuffer->GetBufferSize( ), &inputLayout_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the input layout!" );
        return false;
    }

    ID3DBlob* psBuffer = 0;

    compileResult = CompileD3DShader( "TextureMap.fx", "PS_Main", "ps_4_0", &psBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling pixel shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreatePixelShader( psBuffer->GetBufferPointer( ),
        psBuffer->GetBufferSize( ), 0, &textureMapPS_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating pixel shader!" );
        return false;
    }

////////////////////////////////////////////text 2d shaders////////////////////////////////////////////////////////////

	vsBuffer = 0;

    compileResult = CompileD3DShader( "TextTextureMap.fx", "VS_Main", "vs_4_0", &vsBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling the vertex shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreateVertexShader( vsBuffer->GetBufferPointer( ),
        vsBuffer->GetBufferSize( ), 0, &textTextureMapVS_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the vertex shader!" );

        if( vsBuffer )
            vsBuffer->Release( );

        return false;
    }

    D3D11_INPUT_ELEMENT_DESC textSolidColorLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    totalLayoutElements = ARRAYSIZE( textSolidColorLayout );

    d3dResult = d3dDevice_->CreateInputLayout( textSolidColorLayout, totalLayoutElements,
        vsBuffer->GetBufferPointer( ), vsBuffer->GetBufferSize( ), &textInputLayout_ );

    vsBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the input layout!" );
        return false;
    }

	psBuffer = 0;

    compileResult = CompileD3DShader( "TextTextureMap.fx", "PS_Main", "ps_4_0", &psBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling pixel shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreatePixelShader( psBuffer->GetBufferPointer( ),
        psBuffer->GetBufferSize( ), 0, &textTextureMapPS_ );

    psBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating pixel shader!" );
        return false;
    }
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
	"fontEX.png", 0, 0, &textColorMap_, 0 );


    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }

	D3D11_SAMPLER_DESC textColorMapDesc;
    ZeroMemory( &textColorMapDesc, sizeof( textColorMapDesc ) );
    textColorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    textColorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    textColorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    textColorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    textColorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    textColorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

    d3dResult = d3dDevice_->CreateSamplerState( &textColorMapDesc, &textColorMapSampler_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create color map sampler state!" );
        return false;
    }

    D3D11_BUFFER_DESC textVertexDesc;
    ZeroMemory( &textVertexDesc, sizeof( textVertexDesc ) );
    textVertexDesc.Usage = D3D11_USAGE_DYNAMIC;
    textVertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    textVertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    const int sizeOfSprite = sizeof( TextVertexPos ) * 6;
    const int maxLetters = 24;

    textVertexDesc.ByteWidth = sizeOfSprite * maxLetters;

    d3dResult = d3dDevice_->CreateBuffer( &textVertexDesc, 0, &textVertexBuffer_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create vertex buffer!" );
        return false;
    }

	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Load the models from the file.
    ObjModel objModel;

    if( objModel.LoadOBJ( MODEL_NAME ) == false )
    {
        DXTRACE_MSG( "Error loading 3D model!" );
        return false;
    }

    totalVerts_ = objModel.GetTotalVerts( );

    VertexPos* vertices = new VertexPos[totalVerts_];
    float* vertsPtr = objModel.GetVertices( );
    float* texCPtr = objModel.GetTexCoords( );	
    float* normalPtr = objModel.GetNormals( );

    for( int i = 0; i < totalVerts_; i++ )
    {
        vertices[i].pos = XMFLOAT3( *(vertsPtr + 0), *(vertsPtr + 1), *(vertsPtr + 2) );
        vertsPtr += 3;

        vertices[i].tex0 = XMFLOAT2( *(texCPtr + 0), *(texCPtr + 1) );
        texCPtr += 2;
		
        vertices[i].norm = XMFLOAT3( *(normalPtr + 0), *(normalPtr + 1), *(normalPtr + 2) );
        normalPtr += 3;
    }

    D3D11_BUFFER_DESC vertexDesc;

    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexPos ) * totalVerts_;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory( &resourceData, sizeof( resourceData ) );
    resourceData.pSysMem = vertices;

    d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBuffer_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create vertex buffer!" );
        return false;
    }

    delete[] vertices;
    objModel.Release( );

	VertexPos terrainVertices[] =
    {
        { XMFLOAT3(  100.0f, 0.0f , 100.0f ), XMFLOAT2( 1.0f, 0.0f ), XMFLOAT3(  0.0f, 1.0f , 0.0f ) },
        { XMFLOAT3(  100.0f, 0.0f , -100.0f ), XMFLOAT2( 1.0f, 1.0f ) , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3( -100.0f , -0.0f , -100.0f ), XMFLOAT2( 0.0f, 1.0f ) , XMFLOAT3(  0.0f, 1.0f , 0.0f )},

        { XMFLOAT3( -100.0f , -0.0f , -100.0f ), XMFLOAT2( 0.0f, 1.0f ) , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3( -100.0f ,  0.0f, 100.0f ), XMFLOAT2( 0.0f, 0.0f ) , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3(  100.0f,  0.0f, 100.0f ), XMFLOAT2( 1.0f, 0.0f ) , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
    };
    
	//D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexPos ) * 6;

    //D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory( &resourceData, sizeof( resourceData ) );
    resourceData.pSysMem = terrainVertices;

    d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBufferTerrain_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create vertex buffer!" );
        return false;
    }

	////////////////////////////////////////////////////////////////////////////////////////////////////////

    //load floor and background
	pingu.LoadContent(d3dDevice_,"swamp.jpg");
	savannah.LoadContent(d3dDevice_, "savannah.png");
	drought.LoadContent(d3dDevice_, "drought.jpg");
	rock.LoadContent(d3dDevice_, "rock.png");
	swamp.LoadContent(d3dDevice_, "bgswamp.png");
	projectile.LoadContent(d3dDevice_, "projectile.jpg");

	//load textures
	illuminati.LoadContent(d3dDevice_, "lily.jpg");
	black.LoadContent(d3dDevice_, "ninjatexture.png");
	frogtexture.LoadContent(d3dDevice_, "frogtexture.png");
	rhinotexture.LoadContent(d3dDevice_, "rhinotexture.png");
	

	//ninja
	jninja.LoadMesh(d3dDevice_, "jump.obj");				//jump idle position of player
	cninja.LoadMesh(d3dDevice_, "crouch.obj");				//crouch idle position of player
	ninja.LoadMesh(d3dDevice_, "walk4.obj");				//the player

	//ninja throw
	tninja[0].LoadMesh(d3dDevice_, "throw1.obj");			//throw animation
	tninja[1].LoadMesh(d3dDevice_, "throw2.obj");			//throw animation
	tninja[2].LoadMesh(d3dDevice_, "throw3.obj");			//throw animation
	tninja[3].LoadMesh(d3dDevice_, "throw4.obj");			//throw animation
	tninja[4].LoadMesh(d3dDevice_, "throw5.obj");			//throw animation
	tninja[5].LoadMesh(d3dDevice_, "throw6.obj");			//throw animation
	tninja[6].LoadMesh(d3dDevice_, "throw7.obj");			//throw animation
	tninja[7].LoadMesh(d3dDevice_, "throw8.obj");			//throw animation
	tninja[8].LoadMesh(d3dDevice_, "throw9.obj");			//throw animation
	tninja[9].LoadMesh(d3dDevice_, "throw10.obj");			//throw animation
	tninja[10].LoadMesh(d3dDevice_, "throw11.obj");			//throw animation

	//ninja walk
	wninja[0].LoadMesh(d3dDevice_, "walk1.obj");			//walk animation
	wninja[1].LoadMesh(d3dDevice_, "walk2.obj");			//walk animation
	wninja[2].LoadMesh(d3dDevice_, "walk3.obj");			//walk animation
	wninja[3].LoadMesh(d3dDevice_, "walk4.obj");			//walk animation
	wninja[4].LoadMesh(d3dDevice_, "walk5.obj");			//walk animation
	wninja[5].LoadMesh(d3dDevice_, "walk6.obj");			//walk animation
	wninja[6].LoadMesh(d3dDevice_, "walk7.obj");			//walk animation
	wninja[7].LoadMesh(d3dDevice_, "walk8.obj");			//walk animation
	wninja[8].LoadMesh(d3dDevice_, "walk9.obj");			//walk animation
	wninja[9].LoadMesh(d3dDevice_, "walk10.obj");			//walk animation
	wninja[10].LoadMesh(d3dDevice_, "walk11.obj");			//walk animation

	//shuriken
	shuriken.LoadMesh(d3dDevice_, "shuriken.obj");			//the projectile

	//terrain
	plainmesh.LoadMesh(d3dDevice_, "pad3.obj");				//the lillypad
	plain2mesh.LoadMesh(d3dDevice_, "stone.obj");			//the ground for lvl 2

	//frog
	frogmesh.LoadMesh(d3dDevice_, "frog.obj");				//idle position
	frogjmesh[0].LoadMesh(d3dDevice_, "frogjump1.obj");		//jump animation
	frogjmesh[1].LoadMesh(d3dDevice_, "frogjump2.obj");		//jump animation
	frogjmesh[2].LoadMesh(d3dDevice_, "frogjump3.obj");		//jump animation
	frogjmesh[3].LoadMesh(d3dDevice_, "frogjump4.obj");		//jump animation
	frogjmesh[4].LoadMesh(d3dDevice_, "frogjump5.obj");		//jump animation
	frogjmesh[5].LoadMesh(d3dDevice_, "frogjump6.obj");		//jump animation
	frogjmesh[6].LoadMesh(d3dDevice_, "frogjump7.obj");		//jump animation
	frogjmesh[7].LoadMesh(d3dDevice_, "frogjump8.obj");		//jump animation
	frogjmesh[8].LoadMesh(d3dDevice_, "frogjump9.obj");		//jump animation
	frogjmesh[9].LoadMesh(d3dDevice_, "frogjump10.obj");	//jump animation
	frogjmesh[10].LoadMesh(d3dDevice_, "frogjump11.obj");	//jump animation

	//rhino
	rhinomesh.LoadMesh(d3dDevice_, "rhino.obj");		//the rhino
	wrhino[0].LoadMesh(d3dDevice_, "R1.obj");			//walk animation
	wrhino[1].LoadMesh(d3dDevice_, "R2.obj");			//walk animation
	wrhino[2].LoadMesh(d3dDevice_, "R3.obj");			//walk animation
	wrhino[3].LoadMesh(d3dDevice_, "R4.obj");			//walk animation
	wrhino[4].LoadMesh(d3dDevice_, "R5.obj");			//walk animation
	wrhino[5].LoadMesh(d3dDevice_, "R6.obj");			//walk animation
	wrhino[6].LoadMesh(d3dDevice_, "R7.obj");			//walk animation
	wrhino[7].LoadMesh(d3dDevice_, "R8.obj");			//walk animation
	wrhino[8].LoadMesh(d3dDevice_, "R9.obj");			//walk animation
	wrhino[9].LoadMesh(d3dDevice_, "R10.obj");			//walk animation
	wrhino[10].LoadMesh(d3dDevice_, "R11.obj");			//walk animation

	//setup player starting position, texture, size
	player.SetHealth(5.0f);
	player.setTexture(&black);
	player.setMesh(&ninja);																////PLAYER IS HERE
	player.setPosition(-1, 0, -15);
	player.setSize(1.0);
	player.SetMaxJump(6.5f);
	player.SetJumpspeed(15.0f);
	player.faceright();


	time_t t;
	srand((unsigned) time(&t));

	//setup frog starting position, texture, size
	for (int i = 0; i < NUM_FROGS; i++)
	{
		frogs[i].SetHealth(3);
		frogs[i].setTexture(&frogtexture);
		frogs[i].setMesh(&frogmesh);
		frogs[i].setPosition(5 + i * (rand() % 11 + 4), 1 * (rand() % 10 + 1), -15);
		frogs[i].setSize(1.0);
		frogs[i].faceleft();
	}

	//setup rhino starting position, texture, size
	for (int i = 0; i < NUM_RHINOS; i++)
	{
		rhinos[i].SetHealth(5);
		rhinos[i].setTexture(&rhinotexture);
		rhinos[i].setMesh(&rhinomesh);
		rhinos[i].setPosition(5 + i * (rand() % 11 + 4), 0, -15);
		rhinos[i].setSize(1.0);
		rhinos[i].faceleft();

	}
	//setup shuriken mesh and texture
	for (int b = 0; b < 3; b++)
	{
		bullet[b].setMesh(&shuriken);
		bullet[b].setTexture(&projectile);
		bullet[b].setPosition(XMVectorGetX(player.getPosition()), 2000.0f, 0.0f);

	}
	
	//setup plain
	for (int i = 0; i < NUM_PLAIN; i++)
	{
		plain[i].setMesh(&plainmesh);
		plain[i].setTexture(&illuminati);

		plain[i].setPosition(-20 + i*4.7, 0, -15);
	}

	//setup plain for level 2
	for (int i = 0; i < NUM_GROUND; i++)
	{
		plain2[i].setMesh(&plain2mesh);
		plain2[i].setTexture(&rock);

		plain2[i].setPosition(-20 + i*4.2, 0, -15);
	}

    d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
        "decal.dds", 0, 0, &colorMap_, 0 );



    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }



    D3D11_SAMPLER_DESC colorMapDesc;
    ZeroMemory( &colorMapDesc, sizeof( colorMapDesc ) );
    colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

    d3dResult = d3dDevice_->CreateSamplerState( &colorMapDesc, &colorMapSampler_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create color map sampler state!" );
        return false;
    }


    D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &viewCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &projCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &worldCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

	constDesc.ByteWidth = sizeof( XMFLOAT4 );

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &camPosCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	d3dResult = d3dDevice_->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if(FAILED(d3dResult))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	d3dResult = d3dDevice_->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if(FAILED(d3dResult))
	{
		return false;
	}

    projMatrix_ = XMMatrixPerspectiveFovLH( XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f );
    projMatrix_ = XMMatrixTranspose( projMatrix_ );

    camera_.SetDistance( 12.0f, 4.0f, 20.0f );

    return true;
}


void ModelsDemo::UnloadContent( )
{
	//ninja and terrain meshes
	plainmesh.UnloadMesh();
	plain2mesh.UnloadMesh();
	ninja.UnloadMesh();
	shuriken.UnloadMesh();
	jninja.UnloadMesh();
	cninja.UnloadMesh();
	projectile.UnloadContent();

	//ninja throw animation
	tninja[0].UnloadMesh();			
	tninja[1].UnloadMesh();
	tninja[2].UnloadMesh();
	tninja[3].UnloadMesh();
	tninja[4].UnloadMesh();
	tninja[5].UnloadMesh();
	tninja[6].UnloadMesh();
	tninja[7].UnloadMesh();
	tninja[8].UnloadMesh();
	tninja[9].UnloadMesh();
	tninja[10].UnloadMesh();

	//ninja walk animation
	wninja[0].UnloadMesh();
	wninja[1].UnloadMesh();
	wninja[2].UnloadMesh();
	wninja[3].UnloadMesh();
	wninja[4].UnloadMesh();
	wninja[5].UnloadMesh();
	wninja[6].UnloadMesh();
	wninja[7].UnloadMesh();
	wninja[8].UnloadMesh();
	wninja[9].UnloadMesh();
	wninja[10].UnloadMesh();

	//frog jump animation
	frogmesh.UnloadMesh();
	frogjmesh[0].UnloadMesh();
	frogjmesh[1].UnloadMesh();
	frogjmesh[2].UnloadMesh();
	frogjmesh[3].UnloadMesh();
	frogjmesh[4].UnloadMesh();
	frogjmesh[5].UnloadMesh();
	frogjmesh[6].UnloadMesh();
	frogjmesh[7].UnloadMesh();
	frogjmesh[8].UnloadMesh();
	frogjmesh[9].UnloadMesh();
	frogjmesh[10].UnloadMesh();

	//rhino walk animation
	wrhino[0].UnloadMesh();
	wrhino[1].UnloadMesh();
	wrhino[2].UnloadMesh();
	wrhino[3].UnloadMesh();
	wrhino[4].UnloadMesh();
	wrhino[5].UnloadMesh();
	wrhino[6].UnloadMesh();
	wrhino[7].UnloadMesh();
	wrhino[8].UnloadMesh();
	wrhino[9].UnloadMesh();
	wrhino[10].UnloadMesh();

	//other stuff
	rhinomesh.UnloadMesh();
	swamp.UnloadContent();
	pingu.UnloadContent();
	savannah.UnloadContent();
	drought.UnloadContent();
	rock.UnloadContent();
	sanik.UnloadContent();
	illuminati.UnloadContent();
	frogtexture.UnloadContent();
	rhinotexture.UnloadContent();
	

    if( colorMapSampler_ ) colorMapSampler_->Release( );
	if( textColorMapSampler_ ) textColorMapSampler_->Release( );
    if( colorMap_ ) colorMap_->Release( );
	if( textColorMap_ ) textColorMap_->Release( );
    if( textureMapVS_ ) textureMapVS_->Release( );
    if( textTextureMapPS_ ) textTextureMapPS_->Release( );
    if( textTextureMapVS_ ) textTextureMapVS_->Release( );
    if( textureMapPS_ ) textureMapPS_->Release( );
    if( inputLayout_ ) inputLayout_->Release( );
	if( textInputLayout_ ) textInputLayout_->Release( );
    if( vertexBuffer_ ) vertexBuffer_->Release( );
	if( textVertexBuffer_ ) textVertexBuffer_->Release( );
    if( viewCB_ ) viewCB_->Release( );
    if( projCB_ ) projCB_->Release( );
    if( worldCB_ ) worldCB_->Release( );
	if( camPosCB_ ) camPosCB_->Release( );

	if(m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if(m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}


    colorMapSampler_ = 0;
	textColorMapSampler_ = 0;
    colorMap_ = 0;
	textColorMap_ = 0;
    textureMapVS_ = 0;
    textureMapPS_ = 0;
	textTextureMapVS_ = 0;
    textTextureMapPS_ = 0;
    inputLayout_ = 0;
	textInputLayout_ = 0;
    vertexBuffer_ = 0;
	textVertexBuffer_ = 0;
    viewCB_ = 0;
    projCB_ = 0;
    worldCB_ = 0;
}

void ModelsDemo::TurnOnAlphaBlending()
{
	float blendFactor[4];
	

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn on the alpha blending.
	d3dContext_->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}


void ModelsDemo::TurnOffAlphaBlending()
{
	float blendFactor[4];
	

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn off the alpha blending.
	d3dContext_->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void ModelsDemo::TurnZBufferOn()
{
	d3dContext_->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}


void ModelsDemo::TurnZBufferOff()
{
	d3dContext_->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

void ModelsDemo::SetGameState(GameStates state)
{
	gameState_ = state;
}
GameStates ModelsDemo::GetGameState()
{
	return gameState_;
}

void ModelsDemo::Update( float dt )
{


	float moveSpeed=0.01f;
	float moveSpeed2=5.0f;
	float zoom = 0.0;
	float xRotation=0.0;
	float yRotation=0.0;

	float moveLeftRight =0.0;
	float moveBackForward =0.0;
	float moveJump = 0.0;

	if(gameState_ == START_MENU)
	{

		if(keystate[DIK_RETURN] & 0x80)
		{
			gameState_ = RUN;
		}
	}

	if(gameState_ == PAUSED)
	{
		
		if(!(keystate[DIK_ESCAPE] & 0x80)&&(keyPrevState[DIK_ESCAPE] & 0x80))
		{
			gameState_ = RUN;
		}
		if((keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == RETURN))
		{
			gameState_ = RUN;
		}
		if((keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == PLAY_MOVIE))
		{
			gameState_ = INTRO_MOVIE_REPLAY;
		}
		
		if((keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == QUIT))
		{
			PostQuitMessage(0);
		}
		
		if((!(keystate[DIK_RETURN] & 0x80)&&(keyPrevState[DIK_RETURN] & 0x80))
			&&(pauseMenuSelection == FPS))
		{
			displayFPS = !displayFPS;
		}
		
		if(
			(!(keystate[DIK_DOWN] & 0x80)&&(keyPrevState[DIK_DOWN] & 0x80))
			||
			(!(keystate[DIK_S] & 0x80)&&(keyPrevState[DIK_S] & 0x80))
			)
		{
			pauseMenuSelection++;

		}
		if(
			(!(keystate[DIK_UP] & 0x80)&&(keyPrevState[DIK_UP] & 0x80))
			||
			(!(keystate[DIK_W] & 0x80)&&(keyPrevState[DIK_W] & 0x80))
			)

		{
			pauseMenuSelection--;

		}


	}

	if (gameState_ == RUN || RUN_2)
	{
		switch (playerstate)
		{
			case WALK: //for walking

				wninjaframetime += dt;
			if (wninjaframetime > 0.07f)
			{
				wninjacurrentframe ++;
				wninjaframetime = 0;

				if (wninjacurrentframe > 10)
				{
					wninjacurrentframe = 0;
				}

				player.setMesh(&wninja[wninjacurrentframe]);

			}
				break;


			case THROW: // for throwing

			tninjaframetime += dt;
			if (tninjaframetime > 0.07f)
			{
				tninjacurrentframe++;
				tninjaframetime = 0;

				if (tninjacurrentframe > 10)
				{
					tninjacurrentframe = 0;
				}

				player.setMesh(&tninja[tninjacurrentframe]);

			}

			break;

			case IDLE:
				player.setMesh(&ninja);

				break;

			case JUMP:

				player.setMesh(&jninja);

				break;

			case CROUCH:

				player.setMesh(&cninja);

				break;

			default:

				player.setMesh(&ninja);

		}
	}

	if(gameState_ == RUN)
	{
		
		//go to level 2
		if ((keystate[DIK_2] & 0x80))
		{
			gameState_ = RUN_2;
		}
		
		if (XMVectorGetX(player.getPosition()) > 70)
		{
			gameState_ = WIN;
		}

		//move left
		if((keystate[DIK_LEFT] & 0x80)||(keystate[DIK_A] & 0x80))
		{
			playerstate = WALK;

			moveLeftRight -= moveSpeed2*dt*3;
			player.faceleft();
			player.Move(-moveSpeed2*dt*3);


			if (XMVectorGetX(player.getPosition()) < -15)
			{
				player.Move(2.0f);
				moveLeftRight += 2.0f;
			}

		}

		//move right
		else if((keystate[DIK_RIGHT] & 0x80)||(keystate[DIK_D] & 0x80))
		{
			playerstate = WALK;

			moveLeftRight += moveSpeed2*dt*3;
			player.faceright();
			player.Move(moveSpeed2*dt*3);

			if (XMVectorGetX(player.getPosition()) < -15)
			{
				player.Move(2.0f);
				moveLeftRight += 2.0f;
			}

		}	

		else
		{
			playerstate = IDLE;
		}



		//jump
		if((keystate[DIK_UP] & 0x80)||(keystate[DIK_W] & 0x80))
		{
			player.jump();
			playerstate = JUMP;
		
		}


		//throw shuriken
		if(keystate[DIK_SPACE] & 0x80)
		{
			playerstate = THROW;

			for (int b = 0; b < 3; b++)
			{
				if (bullet[b].isThrown == false && sCounter <= 0)// &&
				{
					sCounter = 1000;
					bullet[b].isThrown = true;

					if (player.isFacingRight())
					{
						bullet[b].faceright();
					}
													
					else
					{
						bullet[b].faceleft();
					}


					bullet[b].setPosition(XMVectorGetX(player.getPosition()), XMVectorGetY(player.getPosition()) + 3.4f, -15.0f);
					bullet[b].setSpeed(5.0);


					if (player.isCrouching())
					{
						bullet[b].setPosition(XMVectorGetX(player.getPosition()), XMVectorGetY(player.getPosition()) + 2.4f, -15.0f);
						bullet[b].setSpeed(5.0);
					}
					
				}
			}
		}

		if ((keystate[DIK_DOWN] & 0x80) || (keystate[DIK_S] & 0x80))
		{

			player.crouch();
			playerstate = CROUCH;

		}

		else
		{
			player.stopcrouch();
		}

		//Update shuriken
		for (int b = 0; b < 3; b++)
		{
			for (int i = 0; i < NUM_FROGS; i++)
			{
				bullet[b].update(dt);

				//get X from bullet, player and enemies
				float bulX = XMVectorGetX(bullet[b].getPosition());
				float bulY = XMVectorGetY(bullet[b].getPosition());

				float plaX = XMVectorGetX(player.getPosition());
				float plaY = XMVectorGetY(player.getPosition());

			
				float frogX = XMVectorGetX(frogs[i].getPosition());
				float frogY = XMVectorGetY(frogs[i].getPosition());


				//calculations
				float temp = fabs(bulX - plaX);

				//makes shuriken spin
				bullet[b].setSpin(3.0f*dt);

				//if the shuriken reaches the frog then it collides and dissapears			
				if (isColliding(bulX, bulY, frogX, frogY, 2.2f))
				{
					bullet[b].isThrown = false;				 //stops bullet
					bullet[b].setPosition(0, 20000.0f, 0);	 //throw bullet away in stratosphere
					frogs[i].getDamage(1);

				}

				//if the shuriken reaches the end of the screen then it dissapears
				if (temp >= 15.0f)							//distance of shuriken regeneration
				{
					bullet[b].isThrown = false;				 //stops bullet
					bullet[b].setPosition(0, 20000.0f, 0);	 //throw bullet away in stratosphere
				}

			}
		}

		//collision of frog with player
		for (int i = 0; i < NUM_FROGS; i++)
		{
			if (isColliding(XMVectorGetX(frogs[i].getPosition()), XMVectorGetY(frogs[i].getPosition()), XMVectorGetX(player.getPosition()), XMVectorGetY(player.getPosition()), 2.2f))
			{
				

				if(frogs[i].isDead() != true)
				{
					player.getDamage(1);

					if (player.isFacingRight())
					{
						player.Move(-2.0f);
						moveLeftRight += -2.0f;
					}

					else
					{
						player.Move(2.0f);
						moveLeftRight += 2.0f;
					}
				}
				
			}
		}
		//update frog
		time_t ti;
		srand((unsigned)time(&ti));
		for (int i = 0; i < NUM_FROGS; i++)
		{
			frogframetime[i] += dt;
			if (frogframetime[i] > 0.07f)
			{
				frogcurrentframe[i] ++;
				frogframetime[i] = 0;

				if (frogcurrentframe[i] > 10)
				{
					frogcurrentframe[i] = 0;
				}

				frogs[i].setMesh(&frogjmesh[frogcurrentframe[i]]);

			}

			float direction = -1;
			if (XMVectorGetX(frogs[i].getPosition()) > XMVectorGetX(player.getPosition()))
			{
				direction = -1;
				frogs[i].faceleft();
				frogs[i].faceleft();
			}
			else
			{
				direction = 1;
				frogs[i].faceright();
				frogs[i].faceright();
			}

			frogs[i].Move(direction*(rand()%3+4) * dt);
			frogs[i].update(dt);
		
		}

		//if player health is 0 then dies 
		if (player.isDead() == true)
		{
			player.setSpin(-2.0f*dt);
			player.MoveY(-1.0f*dt);
			
			if (XMVectorGetY(player.getPosition()) < -0.5)
			{
				gameState_ = LOSE;
			}

		}


		//if frog health is 0 then it dies
		for (int i = 0; i < NUM_FROGS; i++)
		{

			if (frogs[i].isDead() == true)
			{
				frogs[i].setSpin(-3.0f*dt);
				frogs[i].MoveY(-3.0f*dt);

				if (XMVectorGetY(frogs[i].getPosition()) < -3)
				{
					frogs[i].SetHealth(3);
					frogs[i].setPosition(20, 1 * (rand() % 10 + 1), -15);
					frogs[i].resetSpin();

				}

			}

			else
			{
				frogs[i].jump();
			}
		}

		sCounter -=7+dt;


		if(keystate[DIK_ADD] & 0x80)
		{
			zoom -= moveSpeed*2;
		}
	
		if(!(keystate[DIK_ESCAPE] & 0x80)&&(keyPrevState[DIK_ESCAPE] & 0x80))
		{
			gameState_ = PAUSED;
		}
	
	
		if((mouseCurrState.lX != mousePrevState.lX) || (mouseCurrState.lY != mousePrevState.lY))
		{
			yRotation += mousePrevState.lX * 0.005f;

			xRotation += mouseCurrState.lY * 0.005f;

		
			if(xRotation>XM_PI/8)
			{
				xRotation=XM_PI/8;
			}

			if(xRotation<-(XM_PI/8))
			{
				xRotation=-(XM_PI/8);
			}

			mousePrevState = mouseCurrState;
		}


		camera_.Move(moveLeftRight, moveBackForward);

		camera_.ApplyZoom(zoom);

		player.update(dt);
		

	}

	if (gameState_ == WIN)
	{

		if (keystate[DIK_RETURN] & 0x80)
		{
			gameState_ = RUN_2;
		}

	}



	if (gameState_ == RUN_2) //Level 2
	{

		//go to level 1
		if ((keystate[DIK_1] & 0x80))
		{
			gameState_ = RUN;
		}

		if (XMVectorGetX(player.getPosition()) > 60)
		{
			gameState_ = END;
		}

		//move left
		if ((keystate[DIK_LEFT] & 0x80) || (keystate[DIK_A] & 0x80))
		{
			playerstate = WALK;

			moveLeftRight -= moveSpeed2*dt * 3;
			player.faceleft();
			player.Move(-moveSpeed2*dt * 3);

			if (XMVectorGetX(player.getPosition()) < -15)
			{
				player.Move(2.0f);
				moveLeftRight += 2.0f;
			}

		}

		//move right
		else if ((keystate[DIK_RIGHT] & 0x80) || (keystate[DIK_D] & 0x80))
		{
			playerstate = WALK;

			moveLeftRight += moveSpeed2*dt * 3;
			player.faceright();
			player.Move(moveSpeed2*dt * 3);

			if (XMVectorGetX(player.getPosition()) < -15)
			{
				player.Move(2.0f);
				moveLeftRight += 2.0f;
			}
		}


		else
		{
			playerstate = IDLE;
		}



		//jump
		if ((keystate[DIK_UP] & 0x80) || (keystate[DIK_W] & 0x80))
		{
			player.jump();
			playerstate = JUMP;

		}


		//throw shuriken
		if (keystate[DIK_SPACE] & 0x80)
		{
			playerstate = THROW;

			for (int b = 0; b < 3; b++)
			{
				if (bullet[b].isThrown == false && sCounter <= 0)// &&
				{
					sCounter = 1000;
					bullet[b].isThrown = true;

					if (player.isFacingRight())
					{
						bullet[b].faceright();
					}

					else
					{
						bullet[b].faceleft();
					}


					bullet[b].setPosition(XMVectorGetX(player.getPosition()), XMVectorGetY(player.getPosition()) + 3.4f, -15.0f);
					bullet[b].setSpeed(5.0);


					if (player.isCrouching())
					{
						bullet[b].setPosition(XMVectorGetX(player.getPosition()), XMVectorGetY(player.getPosition()) + 2.4f, -15.0f);
						bullet[b].setSpeed(5.0);
					}

				}
			}
		}

		if ((keystate[DIK_DOWN] & 0x80) || (keystate[DIK_S] & 0x80))
		{

			player.crouch();
			playerstate = CROUCH;

		}

		else
		{
			player.stopcrouch();
		}

		//Update shuriken
		for (int b = 0; b < 3; b++)
		{
			for (int i = 0; i < NUM_RHINOS; i++)
			{
				bullet[b].update(dt);

				//get X from bullet, player and enemies
				float bulX = XMVectorGetX(bullet[b].getPosition());
				float bulY = XMVectorGetY(bullet[b].getPosition());

				float plaX = XMVectorGetX(player.getPosition());
				float plaY = XMVectorGetY(player.getPosition());


				float rhinoX = XMVectorGetX(rhinos[i].getPosition());
				float rhinoY = XMVectorGetY(rhinos[i].getPosition());


				//calculations
				float temp = fabs(bulX - plaX);

				//makes shuriken spin
				bullet[b].setSpin(3.0f*dt);

				//if the shuriken reaches the rhino then it collides and dissapears			
				if (isColliding(bulX, bulY, rhinoX, rhinoY, 3.5f))
				{
					bullet[b].isThrown = false;				 //stops bullet
					bullet[b].setPosition(0, 20000.0f, 0);	 //throw bullet away in stratosphere
					rhinos[i].getDamage(1);

				
				}

				//if the shuriken reaches the end of the screen then it dissapears
				if (temp >= 15.0f)							//distance of shuriken regeneration
				{
					bullet[b].isThrown = false;				 //stops bullet
					bullet[b].setPosition(0, 20000.0f, 0);	 //throw bullet away in stratosphere
				}

			}
		}

		//collision of rhinos with player
		for (int i = 0; i < NUM_RHINOS; i++)
		{
			if (isColliding(XMVectorGetX(rhinos[i].getPosition()), XMVectorGetY(rhinos[i].getPosition()), XMVectorGetX(player.getPosition()), XMVectorGetY(player.getPosition()), 2.2f))
			{

				if (rhinos[i].isDead() != true)
				{
					player.getDamage(2);

					if (player.isFacingRight())
					{
						player.Move(-2.0f);
						moveLeftRight += -2.0f;
					}

					else
					{
						player.Move(2.0f);
						moveLeftRight += 2.0f;
					}
				}

			}
		}
		//update rhino
		time_t ti;
		srand((unsigned)time(&ti));
		for (int i = 0; i < NUM_RHINOS; i++)
		{
			

			wrhinoframetime[i] += dt;
			if (wrhinoframetime[i] >0.07)
			{
				wrhinocurrentframe[i] ++;
				wrhinoframetime[i] = 0;

				if (wrhinocurrentframe[i] > 10)
				{
					wrhinocurrentframe[i] = 0;
				}
				
				rhinos[i].setMesh(&wrhino[wrhinocurrentframe[i]]);

			}

			rhinos[i].currentturn = XMVectorGetX(rhinos[i].getPosition()) > XMVectorGetX(player.getPosition());

			if (rhinos[i].previosturn != rhinos[i].currentturn)
			{
				rhinos[i].rhinoturn = true;
				rhinos[i].rhinoturntime = 1.0f;
			}

			if (rhinos[i].rhinoturn == true)
			{
				rhinos[i].rhinoturntime -= dt;
			}

			float direction = -1;
			if (rhinos[i].rhinoturntime <= 0)
			{
				
				if (XMVectorGetX(rhinos[i].getPosition()) > XMVectorGetX(player.getPosition()))
				{
					direction = -1;
					rhinos[i].faceleft();
					rhinos[i].faceleft();
				}

				else
				{
					direction = 1;
					rhinos[i].faceright();
					rhinos[i].faceright();
				}
			}
			rhinos[i].Move(direction* dt);
			rhinos[i].update(dt);
			

			rhinos[i].previosturn = rhinos[i].currentturn;

		}

		//if player health is 0 then dies 
		if (player.isDead() == true)
		{
			player.setSpin(-1.0f*dt);
			player.MoveY(-1.0f*dt);

			if (XMVectorGetY(player.getPosition()) < -1)
			{
				gameState_ = LOSE_2;
			}

		}


		//if rhino health is 0 then it dies
		for (int i = 0; i < NUM_RHINOS; i++)
		{

			if (rhinos[i].isDead() == true)
			{
				rhinos[i].setSpin(-3.0f*dt);
				rhinos[i].MoveY(-3.0f*dt);

				if (XMVectorGetY(rhinos[i].getPosition()) < -3)
				{
					rhinos[i].SetHealth(5);
					rhinos[i].setPosition(XMVectorGetX(player.getPosition()) + 20, 0, -15);
					rhinos[i].resetSpin();

				}

			}

		}

		sCounter -= 7 + dt;


		if (keystate[DIK_ADD] & 0x80)
		{
			zoom -= moveSpeed * 2;
		}

		if (!(keystate[DIK_ESCAPE] & 0x80) && (keyPrevState[DIK_ESCAPE] & 0x80))
		{
			//PostQuitMessage(0);
			gameState_ = PAUSED;
		}

		if ((mouseCurrState.lX != mousePrevState.lX) || (mouseCurrState.lY != mousePrevState.lY))
		{
			yRotation += mousePrevState.lX * 0.005f;
			//yRotation += mousePrevState.lX * dt*2.0f;//mouse movement already based on time

			xRotation += mouseCurrState.lY * 0.005f;
			//xRotation += mouseCurrState.lY * dt*2.0f;/mouse movement already based on time


			if (xRotation>XM_PI / 8)
			{
				xRotation = XM_PI / 8;
			}

			if (xRotation<-(XM_PI / 8))
			{
				xRotation = -(XM_PI / 8);
			}

			mousePrevState = mouseCurrState;
		}


		camera_.Move(moveLeftRight, moveBackForward);

		camera_.ApplyZoom(zoom);

		player.update(dt);

	}


    memcpy( &prevController1State_, &controller1State_, sizeof( XINPUT_STATE ) );
}

bool ModelsDemo::isColliding(float x1, float y1, float x2, float y2, float distance)
{
	float a = x1 - x2;
	float b = y1 - y2;
	float result = sqrt(a * a + b * b);

	if (result < distance)
	{
		return true;
	}

	else
	{
		return false;
	}

}

bool ModelsDemo::DrawString( char* message, float startX, float startY )
{
    // Size in bytes for a single sprite.
    const int sizeOfSprite = sizeof( TextVertexPos ) * 6;

    // Demo's dynamic buffer setup for max of 24 letters.
    const int maxLetters = 24;

    int length = strlen( message );

    // Clamp for strings too long.
    if( length > maxLetters )
        length = maxLetters;

    // Char's width on screen.
    float charWidth = 32.0f / 800.0f;

    // Char's height on screen.
    float charHeight = 32.0f / 640.0f;
    
    // Char's texel width.
	float texelWidth = 32.0f / 3072.0f;

    // verts per-triangle (3) * total triangles (2) = 6.
    const int verticesPerLetter = 6;

    D3D11_MAPPED_SUBRESOURCE mapResource;
    HRESULT d3dResult = d3dContext_->Map( textVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to map resource!" );
        return false;
    }

    // Point to our vertex buffer's internal data.
    TextVertexPos *spritePtr = ( TextVertexPos* )mapResource.pData;

	const int indexSpace = static_cast<char>( ' ' );
    const int indexA = static_cast<char>( 'A' );
    const int indexZ = static_cast<char>( 'Z' );
	const int indexSquare = static_cast<char>( 127 );

    for( int i = 0; i < length; ++i )
    {
        float thisStartX = startX + ( charWidth * static_cast<float>( i ) );
        float thisEndX = thisStartX + charWidth;
        float thisEndY = startY + charHeight;

        spritePtr[0].pos = XMFLOAT3( thisEndX,   thisEndY, 1.0f );
        spritePtr[1].pos = XMFLOAT3( thisEndX,   startY,   1.0f );
        spritePtr[2].pos = XMFLOAT3( thisStartX, startY,   1.0f );
        spritePtr[3].pos = XMFLOAT3( thisStartX, startY,   1.0f );
        spritePtr[4].pos = XMFLOAT3( thisStartX, thisEndY, 1.0f );
        spritePtr[5].pos = XMFLOAT3( thisEndX,   thisEndY, 1.0f );

        int texLookup = 0;
        int letter = static_cast<char>( message[i] );

        //if( letter < indexA || letter > indexZ )
        if( letter < indexSpace || letter > indexSquare )
		{
            // Grab one index past Z, which is a blank space in the texture.
           // texLookup = ( indexZ - indexA ) + 1;
			texLookup = indexSquare;
        }
        else
        {
            // A = 0, B = 1, Z = 25, etc.
            //texLookup = ( letter - indexA );
			texLookup = ( letter - indexSpace ); 
        }

        float tuStart = 0.0f + ( texelWidth * static_cast<float>( texLookup ) );
        float tuEnd = tuStart + texelWidth;

        spritePtr[0].tex0 = XMFLOAT2( tuEnd, 0.0f );
        spritePtr[1].tex0 = XMFLOAT2( tuEnd, 1.0f );
        spritePtr[2].tex0 = XMFLOAT2( tuStart, 1.0f );
        spritePtr[3].tex0 = XMFLOAT2( tuStart, 1.0f );
        spritePtr[4].tex0 = XMFLOAT2( tuStart, 0.0f );
        spritePtr[5].tex0 = XMFLOAT2( tuEnd, 0.0f );

        spritePtr += 6;
    }

    d3dContext_->Unmap( textVertexBuffer_, 0 );
    d3dContext_->Draw( 6 * length, 0 );

    return true;
}

void ModelsDemo::Render( )
{

    if( d3dContext_ == 0 )
        return;
	

    //float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	float clearColor[4] = { 0.7f, 0.8f, 1.0f, 1.0f };
    d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );
    d3dContext_->ClearDepthStencilView( depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0 );
			
	unsigned int stride = sizeof( VertexPos );
	unsigned int offset = 0;
	
	if (gameState_ == START_MENU)
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;

		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);


		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);


		DrawString("PRESS ENTER to START", -0.4f, 0.0f);

		TurnOffAlphaBlending();
		TurnZBufferOn();

		////////////////////////////////////////////////////////////////////////////////////////////////
	}

	if (gameState_ == WIN)
	{
			player.setPosition(-1, 0, -15);
			camera_.SetPosition(3.0f, 3.0f, -35.0f);

			TurnZBufferOff();
			TurnOnAlphaBlending();

			stride = sizeof(TextVertexPos);
			offset = 0;

			d3dContext_->IASetInputLayout(textInputLayout_);
			d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
			d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
			d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);


			d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
			d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);


			DrawString("YOU WIN! PRESS ENTER!", -0.4f, 0.0f);


			TurnOffAlphaBlending();
			TurnZBufferOn();

			if (keystate[DIK_RETURN] & 0x80)
			{
				gameState_ = RUN_2;
			}


	}

	if (gameState_ == LOSE)
	{
		player.setPosition(-1, 0, -15);
		player.SetHealth(5.0f);
		player.faceright();
		player.resetSpin();

		camera_.SetPosition(3.0f, 3.0f, -35.0f);

		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;

		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);


		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);


		DrawString("YOU LOSE! PRESS ENTER!", -0.4f, 0.0f);


		TurnOffAlphaBlending();
		TurnZBufferOn();

		if (keystate[DIK_RETURN] & 0x80)
		{
			gameState_ = RUN;
		}

	}

	if (gameState_ == LOSE_2)
	{
		player.setPosition(-1, 0, -15);
		player.SetHealth(5.0f);
		player.faceright();
		player.resetSpin();

		camera_.SetPosition(3.0f, 3.0f, -35.0f);

		stride = sizeof(TextVertexPos);
		offset = 0;

		TurnZBufferOff();
		TurnOnAlphaBlending();

		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);


		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);


		DrawString("YOU LOSE! PRESS ENTER!", -0.4f, 0.0f);


		TurnOffAlphaBlending();
		TurnZBufferOn();

		if (keystate[DIK_RETURN] & 0x80)
		{
			gameState_ = RUN_2;
		}

	}

	if (gameState_ == END)
	{
		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;

		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);


		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);


		DrawString("GAME OVER! PRESS ENTER!", -0.4f, 0.0f);


		TurnOffAlphaBlending();
		TurnZBufferOn();

		if (keystate[DIK_RETURN] & 0x80)
		{
			exit(0);
		}


	}

	if ((gameState_ == RUN))
	{
		
		/////////////////////////////////////////geometry settings//////////////////////////////


		d3dContext_->IASetInputLayout(inputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);

		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &colorMap_);
		d3dContext_->PSSetSamplers(0, 1, &colorMapSampler_);


		//TurnZBufferOff();

		XMMATRIX worldMat = XMMatrixIdentity();
		worldMat = XMMatrixTranspose(worldMat);

		XMMATRIX viewMat = camera_.GetViewMatrix();
		viewMat = XMMatrixTranspose(viewMat);

		XMFLOAT3 cameraPos = camera_.GetPosition();

		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->UpdateSubresource(viewCB_, 0, 0, &viewMat, 0, 0);
		d3dContext_->UpdateSubresource(projCB_, 0, 0, &projMatrix_, 0, 0);
		d3dContext_->UpdateSubresource(camPosCB_, 0, 0, &cameraPos, 0, 0);

		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->VSSetConstantBuffers(1, 1, &viewCB_);
		d3dContext_->VSSetConstantBuffers(2, 1, &projCB_);
		d3dContext_->VSSetConstantBuffers(3, 1, &camPosCB_);

		//d3dContext_->Draw( totalVerts_, 0 );



		///////////////////second object///////////////////////
		static float x = -4.0f;
		static float y = 0.5f;
		static float z = -10.0f;
		//worldMat = XMMatrixTranslation(-4.0f,0.0f,-10.0f);
		worldMat = XMMatrixTranslation(x, y, z);

		if (gameState_ == RUN)


		worldMat = XMMatrixTranspose(worldMat);
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->PSSetShaderResources(0, 1, frogtexture.Render());
		for (int i = 0; i < NUM_FROGS; i++)
		{
			d3dContext_->IASetVertexBuffers(0, 1, frogs[i].getMesh()->Render(), &stride, &offset);

			d3dContext_->UpdateSubresource(worldCB_, 0, 0, frogs[i].getWorldmat(), 0, 0);

			d3dContext_->Draw(frogs[i].getMesh()->gettotalVertex(), 0);
		}
		//////////////////////////////////////////



		static float x2 = 10.0f;
		static float y2 = 5.0f;
		worldMat = XMMatrixTranslation(x2, y2, z);

		if (gameState_ == RUN)

		worldMat = XMMatrixTranspose(worldMat);
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->PSSetShaderResources(0, 1, illuminati.Render());
		//d3dContext_->Draw(ninja.gettotalVertex(), 0);
		///////////////////////////////////////////////////////////////////////

		d3dContext_->IASetVertexBuffers(0, 1, plain[0].getMesh()->Render(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, plain[0].getTexture()->Render());


		for (int i = 0; i < NUM_PLAIN; i++)
		{
			d3dContext_->UpdateSubresource(worldCB_, 0, 0, plain[i].getWorldmat(), 0, 0);


			d3dContext_->Draw(plain[i].getMesh()->gettotalVertex(), 0);

		}


		d3dContext_->IASetVertexBuffers(0, 1, player.getMesh()->Render(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, player.getTexture()->Render());

		d3dContext_->UpdateSubresource(worldCB_, 0, 0, player.getWorldmat(), 0, 0);
		d3dContext_->Draw(player.getMesh()->gettotalVertex(), 0);

		for (int b = 0; b < 3; b++)
		{

			d3dContext_->IASetVertexBuffers(0, 1, bullet[b].getMesh()->Render(), &stride, &offset);
			d3dContext_->PSSetShaderResources(0, 1, bullet[b].getTexture()->Render());

			d3dContext_->UpdateSubresource(worldCB_, 0, 0, bullet[b].getWorldmat(), 0, 0);
			d3dContext_->Draw(bullet[b].getMesh()->gettotalVertex(), 0);

		}


		////////////////////terrain////////////////////////////////
		worldMat = XMMatrixIdentity();
		worldMat = XMMatrixTranspose(worldMat);
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->IASetVertexBuffers(0, 1, &vertexBufferTerrain_, &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, pingu.Render());

		d3dContext_->Draw(6, 0);


		////////////////////background////////////////////////////////
		worldMat = XMMatrixIdentity();
		XMMATRIX BGRotation = XMMatrixRotationX(XM_PIDIV2 + XM_PI);
		XMMATRIX BGTranslation = XMMatrixTranslation(0.0f, 100.0f, 15.0f);
		XMMATRIX BGScale = XMMatrixScaling(1.0, 0.11, 1.0);
		worldMat = BGRotation * BGTranslation * BGScale;
		worldMat = XMMatrixTranspose(worldMat);
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->IASetVertexBuffers(0, 1, &vertexBufferTerrain_, &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, swamp.Render());

		d3dContext_->Draw(6, 0);

		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;


		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);

		char output[64];
		sprintf_s(output, "Player Health:%d", player.getHealth());

		DrawString(output, -0.9f, 0.4f);

		for (int i = 0; i < NUM_FROGS; i++)
		{
		
		sprintf_s(output, "Frog Health:%d", frogs[i].getHealth());

		DrawString(output, 0.3f, 0.4f);

		TurnOffAlphaBlending();
		TurnZBufferOn();
		}
	}

	if((gameState_ == RUN)&&(displayFPS==true))
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		TurnZBufferOff();
		TurnOnAlphaBlending();
	
		stride = sizeof( TextVertexPos );
		offset = 0;


		d3dContext_->IASetInputLayout( textInputLayout_ );
		d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );
		d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		d3dContext_->VSSetShader( textTextureMapVS_, 0, 0 );
		d3dContext_->PSSetShader( textTextureMapPS_, 0, 0 );
		d3dContext_->PSSetShaderResources( 0, 1, &textColorMap_ );
		d3dContext_->PSSetSamplers( 0, 1, &textColorMapSampler_ );

		//char output[24];
		char output[64];
		sprintf_s(output, "FPS:%d",fps_);

		DrawString( output, -0.9f, 0.83f);

		sprintf_s(output, "Frame Time:%.6f", frameTime_);

		DrawString( output, -0.9f, 0.6f);

		TurnOffAlphaBlending();
		TurnZBufferOn();

	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if (gameState_ == RUN_2) //Level 2
	{
		/////////////////////////////////////////geometry settings//////////////////////////////


		d3dContext_->IASetInputLayout(inputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);

		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &colorMap_);
		d3dContext_->PSSetSamplers(0, 1, &colorMapSampler_);


		//TurnZBufferOff();

		XMMATRIX worldMat = XMMatrixIdentity();
		worldMat = XMMatrixTranspose(worldMat);

		XMMATRIX viewMat = camera_.GetViewMatrix();
		viewMat = XMMatrixTranspose(viewMat);

		XMFLOAT3 cameraPos = camera_.GetPosition();

		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->UpdateSubresource(viewCB_, 0, 0, &viewMat, 0, 0);
		d3dContext_->UpdateSubresource(projCB_, 0, 0, &projMatrix_, 0, 0);
		d3dContext_->UpdateSubresource(camPosCB_, 0, 0, &cameraPos, 0, 0);

		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->VSSetConstantBuffers(1, 1, &viewCB_);
		d3dContext_->VSSetConstantBuffers(2, 1, &projCB_);
		d3dContext_->VSSetConstantBuffers(3, 1, &camPosCB_);

		//d3dContext_->Draw( totalVerts_, 0 );


		///////////////////second object///////////////////////
		static float x = -4.0f;
		static float y = 0.5f;
		static float z = -10.0f;
		//worldMat = XMMatrixTranslation(-4.0f,0.0f,-10.0f);
		worldMat = XMMatrixTranslation(x, y, z);

		if (gameState_ == RUN_2)
			

		worldMat = XMMatrixTranspose(worldMat);
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->PSSetShaderResources(0, 1, rhinotexture.Render());
		for (int i = 0; i < NUM_RHINOS; i++)
		{
			d3dContext_->IASetVertexBuffers(0, 1, rhinos[i].getMesh()->Render(), &stride, &offset);

			d3dContext_->UpdateSubresource(worldCB_, 0, 0, rhinos[i].getWorldmat(), 0, 0);

			d3dContext_->Draw(rhinos[i].getMesh()->gettotalVertex(), 0);
		}
		//////////////////////////////////////////



		static float x2 = 10.0f;
		static float y2 = 5.0f;
		worldMat = XMMatrixTranslation(x2, y2, z);

		if (gameState_ == RUN_2)
			

		worldMat = XMMatrixTranspose(worldMat);
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->PSSetShaderResources(0, 1, illuminati.Render());
		//d3dContext_->Draw(ninja.gettotalVertex(), 0);
		///////////////////////////////////////////////////////////////////////

		d3dContext_->IASetVertexBuffers(0, 1, plain2[0].getMesh()->Render(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, plain2[0].getTexture()->Render());


		for (int i = 0; i < NUM_GROUND; i++)
		{
			d3dContext_->UpdateSubresource(worldCB_, 0, 0, plain2[i].getWorldmat(), 0, 0);

			d3dContext_->Draw(plain2[i].getMesh()->gettotalVertex(), 0);

		}

		d3dContext_->IASetVertexBuffers(0, 1, player.getMesh()->Render(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, player.getTexture()->Render());

		d3dContext_->UpdateSubresource(worldCB_, 0, 0, player.getWorldmat(), 0, 0);
		d3dContext_->Draw(player.getMesh()->gettotalVertex(), 0);

		for (int b = 0; b < 3; b++)
		{

			d3dContext_->IASetVertexBuffers(0, 1, bullet[b].getMesh()->Render(), &stride, &offset);
			d3dContext_->PSSetShaderResources(0, 1, bullet[b].getTexture()->Render());

			d3dContext_->UpdateSubresource(worldCB_, 0, 0, bullet[b].getWorldmat(), 0, 0);
			d3dContext_->Draw(bullet[b].getMesh()->gettotalVertex(), 0);

		}


		////////////////////terrain////////////////////////////////
		worldMat = XMMatrixIdentity();
		worldMat = XMMatrixTranspose(worldMat);
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->IASetVertexBuffers(0, 1, &vertexBufferTerrain_, &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, drought.Render());

		d3dContext_->Draw(6, 0);


		////////////////////background////////////////////////////////
		worldMat = XMMatrixIdentity();
		XMMATRIX BGRotation = XMMatrixRotationX(XM_PIDIV2 + XM_PI);
		XMMATRIX BGTranslation = XMMatrixTranslation(0.0f, 100.0f, 15.0f);
		XMMATRIX BGScale = XMMatrixScaling(1.0, 0.11, 1.0);
		worldMat = BGRotation * BGTranslation * BGScale;
		worldMat = XMMatrixTranspose(worldMat);
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->IASetVertexBuffers(0, 1, &vertexBufferTerrain_, &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, savannah.Render());

		d3dContext_->Draw(6, 0);

		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;


		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);

		char output[64];
		sprintf_s(output, "Player Health:%d", player.getHealth());

		DrawString(output, -0.9f, 0.4f);

		for (int i = 0; i < NUM_RHINOS; i++)
		{

			sprintf_s(output, "Rhino Health:%d", rhinos[i].getHealth());

			DrawString(output, 0.3f, 0.4f);

			TurnOffAlphaBlending();
			TurnZBufferOn();
		}

	}

	if ((gameState_ == RUN_2) && (displayFPS == true))
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;


		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);

		char output[64];
		sprintf_s(output, "FPS:%d", fps_);

		DrawString(output, -0.9f, 0.83f);

		sprintf_s(output, "Frame Time:%.6f", frameTime_);

		DrawString(output, -0.9f, 0.6f);

		TurnOffAlphaBlending();
		TurnZBufferOn();

		////////////////////////////////////////////////////////////////////////////////////////////////
	}

	if (gameState_ == WIN)
	{
		char output[64];
		sprintf_s(output, "YOU WIN\n Press Enter to go to next level");
		DrawString(output, -0.9f, 0.83f);
	}

	if (gameState_ == PAUSED)
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		TurnZBufferOff();
		TurnOnAlphaBlending();
	
		stride = sizeof( TextVertexPos );
		offset = 0;

		d3dContext_->IASetInputLayout( textInputLayout_ );
		d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );
		d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		d3dContext_->VSSetShader( textTextureMapVS_, 0, 0 );
		d3dContext_->PSSetShader( textTextureMapPS_, 0, 0 );
		d3dContext_->PSSetShaderResources( 0, 1, &textColorMap_ );
		d3dContext_->PSSetSamplers( 0, 1, &textColorMapSampler_ );


		DrawString( "GAME PAUSED", -0.2f, 0.0f );
		if (pauseMenuSelection == RETURN)
		{
			DrawString( "->Return to Game<-", -0.33f, -0.1f );
		}
		else
		{
			DrawString( "Return to Game", -0.25f, -0.1f );
		}

		if (pauseMenuSelection == FPS)
		{
			if(displayFPS==true)
			{
				DrawString( "->Display FPS: ON<-",-0.35f, -0.2f );
			}
			else
			{
				DrawString( "->Display FPS:OFF<-",-0.35f, -0.2f );
			}
		}
		else
		{
			if(displayFPS==true)
			{
				DrawString( "Display FPS: ON",-0.27f, -0.2f );
			}
			else
			{
				DrawString( "Display FPS:OFF",-0.27f, -0.2f );
			}
		}
		
		if (pauseMenuSelection == PLAY_MOVIE)
		{
			DrawString( "->Play the Movie<-", -0.33f, -0.3f );
		}
		else
		{
			DrawString( "Play the Movie", -0.25f, -0.3f );
		}

		if (pauseMenuSelection == QUIT)
		{
			DrawString( "->Quit the Game<-", -0.33f, -0.4f );
		}
		else
		{
			DrawString( "Quit the Game", -0.25f, -0.4f );
		}


		TurnOffAlphaBlending();
		TurnZBufferOn();

		////////////////////////////////////////////////////////////////////////////////////////////////
	}

    swapChain_->Present( 0, 0 );
}

