////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <list>
#include <atlstr.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "boardclass.h"
#include "cameraclass.h"
#include "instanceshaderclass.h"
#include "resourcesclass.h"
#include "stateclass.h"
#include "textclass.h"
#include "textureshaderclass.h"
#include "triangleclass.h"


///////////////
// FILEPATHS //
///////////////
#define COLORSPATH "../Puzzle/data/colors.txt"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND, StateClass*);
	void Shutdown();
	bool Frame(float, int, float);

private:
	bool Render();

private:
	ResourcesClass*					m_Resources;
	TextClass*						m_Text;
	CameraClass*					m_Camera;
	std::list<DrawableInterface*>	m_Drawables;
	BoardClass*						m_Board;
	TextureShaderClass*				m_TextureShader;
	InstanceShaderClass*			m_InstanceShader;
};
