////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <atlstr.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "resourcesclass.h"
#include "cameraclass.h"
#include "boardclass.h"
#include "colorshaderclass.h"
#include "textclass.h"


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

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float, int, float);

private:
	bool Render();

private:
	ResourcesClass*		m_Resources;
	TextClass*			m_Text;
	CameraClass*		m_Camera;
	BoardClass*			m_Geometry;
	ColorShaderClass*	m_ColorShader;
};
