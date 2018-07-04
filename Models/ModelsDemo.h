/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    3D Models Demo - Demonstrates loading a model from an .OBJ file.
*/


#ifndef _MODELS_DEMO_H_
#define _MODELS_DEMO_H_
#define NUM_FROGS 2
#define NUM_RHINOS 5
#define NUM_PLAIN 50
#define NUM_GROUND 50

enum animation { WALK, THROW, IDLE, JUMP, CROUCH};


#include"Dx11DemoBase.h"
#include"ArcCamera.h"
#include "FirstPersonCamera.h"
#include <xnamath.h>
#include <XInput.h>
#include "Texture.h"
#include "Vertexes.h"
#include "Mesh.h"
#include "GameObject.h"
#include "MovingObject.h"
#include "Shuriken.h"
#include "Frog.h"
#include "Rhino.h"

enum GameStates {PLAY_INTRO, START_MENU, RUN, RUN_2, PAUSED, INTRO_MOVIE_REPLAY, WIN, END, LOSE, LOSE_2};

enum PauseMenuSelection {RETURN, FPS, PLAY_MOVIE, QUIT};
#define PAUSE_MENU_ITEMS 4

inline PauseMenuSelection operator++(PauseMenuSelection &eDOW, int)
{
   const PauseMenuSelection ePrev = eDOW;
   const int i = static_cast<int>(eDOW);
   eDOW = static_cast<PauseMenuSelection>((i + 1) % PAUSE_MENU_ITEMS);
   return ePrev;
}
inline PauseMenuSelection operator--(PauseMenuSelection &eDOW, int)
{
   const PauseMenuSelection ePrev = eDOW;
   const int i = static_cast<int>(eDOW);
   if(i > 0)
   {
		eDOW = static_cast<PauseMenuSelection>(i - 1);
   }
   else
   {
	   eDOW = static_cast<PauseMenuSelection>(PAUSE_MENU_ITEMS - 1);
   }
   return ePrev;
}



class ModelsDemo : public Dx11DemoBase
{
    public:
        ModelsDemo( );
        virtual ~ModelsDemo( );

        bool LoadContent( );
        void UnloadContent( );

		void TurnZBufferOn();
		void TurnZBufferOff();

		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		bool DrawString( char* message, float startX, float startY );
        void Update( float dt );
        void Render( );

		bool isColliding(float, float, float, float, float);

		void SetGameState(GameStates state);
		GameStates GetGameState();

    private:
        ID3D11VertexShader* textureMapVS_;
        ID3D11PixelShader* textureMapPS_;
		ID3D11VertexShader* textTextureMapVS_;
        ID3D11PixelShader* textTextureMapPS_;

        ID3D11InputLayout* inputLayout_;
		ID3D11InputLayout* textInputLayout_;
        ID3D11Buffer* vertexBuffer_;
		ID3D11Buffer* textVertexBuffer_;
		ID3D11Buffer* vertexBufferTerrain_;
		//ID3D11Buffer* vertexBufferBackground_;
		int totalVerts_;


        ID3D11ShaderResourceView* colorMap_;
		ID3D11ShaderResourceView* textColorMap_;

		Texture swamp;
		Texture savannah;
		Texture pingu;
		Texture drought;
		Texture rock;

		Texture sanik;
		Texture illuminati;
		Texture black;
		Texture projectile;

		Texture rhinotexture;
		Texture frogtexture;
		
		Mesh jninja;
		Mesh cninja;
		Mesh ninja;
		Mesh tninja[11];
		Mesh wninja[11];

		Mesh shuriken;
		Shuriken bullet[3];

		Mesh frogmesh;

		Mesh frogjmesh[11];

		int frogcurrentframe[NUM_FROGS];
		float frogframetime[NUM_FROGS];

		int wninjacurrentframe;
		float wninjaframetime;

		int tninjacurrentframe;
		float tninjaframetime;

		int wrhinocurrentframe[NUM_RHINOS];
		float wrhinoframetime[NUM_RHINOS];

		Frog frogs[NUM_FROGS];

		Mesh rhinomesh;
		Rhino rhinos[NUM_RHINOS];

		Mesh wrhino[11];

		float sCounter;

		MovingObject player;

		Mesh plainmesh;
		Mesh plain2mesh;

		GameObject plain[NUM_PLAIN];
		GameObject plain2[NUM_GROUND];

        ID3D11SamplerState* colorMapSampler_;
		ID3D11SamplerState* textColorMapSampler_;

        ID3D11Buffer* viewCB_;
        ID3D11Buffer* projCB_;
        ID3D11Buffer* worldCB_;
        ID3D11Buffer* camPosCB_;
        XMMATRIX projMatrix_;

        //ArcCamera camera_;
		FirstPersonCamera camera_;

        XINPUT_STATE controller1State_;
        XINPUT_STATE prevController1State_;

		GameStates gameState_;
		PauseMenuSelection pauseMenuSelection;
		bool displayFPS;

		//////////////time globals///////////////////
public:
		//double countsPerSecond_ = 0.0;
		//__int64 CounterStart_ = 0;
		//int frameCount_ = 0;
		int fps_;

		//__int64 frameTimeOld_ = 0;
		double frameTime_;
};

#endif