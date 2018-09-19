////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Resources = nullptr;
	m_Text = nullptr;
	m_Camera = nullptr;
	m_Triangle = nullptr;
	m_ColorShader = nullptr;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_Resources = new ResourcesClass;
	if (!m_Resources)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Resources->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Resources->GetDirectWriteFactory(), m_Resources->GetDirect2DDeviceContext(), 20.0f, L"Consolas");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Set the window of the text object.
	m_Text->SetDrawWindow(3, 0, 150, 150);

	// Set the color of our text object.
	m_Text->SetBrushColor(D2D1::ColorF(D2D1::ColorF::Plum));

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	//// Create the board object.
	//m_Drawables.push_front(new BoardClass);
	//if (!m_Drawables.front())
	//{
	//	return false;
	//}

	//// Initialize the board object.
	//result = m_Drawables.front()->Initialize(m_Resources->GetDirect3DDevice(), m_Resources->GetDirect3DDeviceContext());
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the board object.", L"Error", MB_OK);
	//	return false;
	//}

	//// Create the triangle object.
	//m_Drawables.push_front(new TriangleClass);
	//if (!m_Drawables.front())
	//{
	//	return false;
	//}

	//// Initialize the triangle object.
	//result = m_Drawables.front()->Initialize(m_Resources->GetDirect3DDevice(), m_Resources->GetDirect3DDeviceContext());
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the triangle object.", L"Error", MB_OK);
	//	return false;
	//}

	// Create the triangle object.
	m_Triangle = new TriangleClass;
	if (!m_Triangle)
	{
		return false;
	}

	// Initialize the triangle object.
	result = m_Triangle->Initialize(m_Resources->GetDirect3DDevice(), m_Resources->GetDirect3DDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the triangle object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_Resources->GetDirect3DDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = nullptr;
	}

	//// Release the drawable objects.
	//for (auto drawable : m_Drawables)
	//{
	//	if (drawable)
	//	{
	//		drawable->Shutdown();
	//		delete drawable;
	//		drawable = nullptr;
	//	}
	//}

	// Release the triangle object.
	if (m_Triangle)
	{
		m_Triangle->Shutdown();
		delete m_Triangle;
		m_Triangle = nullptr;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = nullptr;
	}

	// Release the Resources object.
	if (m_Resources)
	{
		m_Resources->Shutdown();
		delete m_Resources;
		m_Resources = nullptr;
	}

	return;
}


bool GraphicsClass::Frame(float fps, int cpu, float frameTime)
{
	bool result;
	CString stats;


	// Build the statistics string.
	stats.Format(L"FPS: %.1f\nCPU: %d%%", fps, cpu);
	
	// Set the stats text string for our text object.
	m_Text->SetTextString(stats.GetString());

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Resources->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Resources->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Resources->GetProjectionMatrix(projectionMatrix);

	// Turn on alpha blending for the transparency to work.
	m_Resources->TurnOnAlphaBlending();

	//// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//for (auto drawable : m_Drawables)
	//{
	//	drawable->Render(m_Resources->GetDirect3DDeviceContext());

	//	// Render the model using the color shader.
	//	result = m_ColorShader->Render(m_Resources->GetDirect3DDeviceContext(), drawable->GetIndexCount(), m_Geometry->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix);
	//	if (!result)
	//	{
	//		return false;
	//	}
	//}

	m_Triangle->Render(m_Resources->GetDirect3DDeviceContext());

	// Render the model using the color shader.
	result = m_ColorShader->Render(m_Resources->GetDirect3DDeviceContext(), m_Triangle->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Render text on the screen.
	m_Text->Render(m_Resources->GetDirect2DDeviceContext());

	// Turn off alpha blending.
	m_Resources->TurnOffAlphaBlending();

	// Present the rendered scene to the screen.
	m_Resources->EndScene();

	return true;
}
