////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Resources = nullptr;
	m_Text = nullptr;
	m_Camera = nullptr;
	m_Background = nullptr;
	m_Board = nullptr;
	m_TextureShader = nullptr;
	m_InstanceShader = nullptr;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd, StateClass* state)
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

	// Set the initial parameters of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -20.0f);
	m_Camera->SetLookDirection(0.0f, 0.0f, 1.0f);

	// Create the triangle object.
	m_Background = new QuadClass;
	if (!m_Background)
	{
		return false;
	}

	// Initialize the triangle object.
	result = m_Background->Initialize(m_Resources->GetDirect3DDevice(), m_Resources->GetDirect3DDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the background object.", L"Error", MB_OK);
		return false;
	}

	// Create the board object.
	m_Board = new BoardClass();
	if (!m_Board)
	{
		return false;
	}

	// Initialize the board object.
	result = m_Board->Initialize(m_Resources->GetDirect3DDevice(), m_Resources->GetDirect3DDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the board object.", L"Error", MB_OK);
		return false;
	}

	// Load the board's colors from file.
	result = m_Board->LoadColors(COLORSPATH);
	if (!result)
	{
		MessageBox(hwnd, L"Could not load the board colors from file.", L"Error", MB_OK);
	}

	// Create the instance shader object.
	m_InstanceShader = new InstanceShaderClass;
	if (!m_InstanceShader)
	{
		return false;
	}

	// Initialize the instance shader object.
	result = m_InstanceShader->Initialize(m_Resources->GetDirect3DDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the instance shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_TextureShader->Initialize(m_Resources->GetDirect3DDevice());
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
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

	// Release the instance shader object.
	if (m_InstanceShader)
	{
		m_InstanceShader->Shutdown();
		delete m_InstanceShader;
		m_InstanceShader = nullptr;
	}

	// Release the background model object.
	if (m_Background)
	{
		m_Background->Shutdown();
		delete m_Background;
		m_Background = nullptr;
	}

	// Release the board object.
	if (m_Board)
	{
		m_Board->Shutdown();
		delete m_Board;
		m_Board = nullptr;
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


bool GraphicsClass::Frame(float fps, int cpu, float frameTime, StateClass* state)
{
	bool result;
	CString stats;


	// Build the statistics string.
	stats.Format(L"FPS: %.1f\nCPU: %d%%", fps, cpu);
	
	// Set the stats text string for our text object.
	m_Text->SetTextString(stats.GetString());

	// Initialize the board object's level.
	result = m_Board->CreateInstances(m_Resources->GetDirect3DDevice(), state);
	if (!result)
	{
		return false;
	}

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

	// Turn on alpha blending for the transparency to work.
	m_Resources->TurnOnAlphaBlending();

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Resources->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Resources->GetProjectionMatrix(projectionMatrix);

	// Submit our background geometry buffers to the device for rendering.
	m_Background->Render(m_Resources->GetDirect3DDeviceContext());

	// Render the Background using the texture shader.
	result = m_TextureShader->Render(m_Resources->GetDirect3DDeviceContext(), m_Background->GetIndexCount(), 1, 
									worldMatrix, viewMatrix, projectionMatrix, m_Background->GetTexture());
	if (!result)
	{
		return false;
	}

	// Submit our board geometry buffers to the device for rendering.
	m_Board->Render(m_Resources->GetDirect3DDeviceContext());

	// Render the board using the instance shader.
	result = m_InstanceShader->Render(m_Resources->GetDirect3DDeviceContext(), m_Board->GetIndexCount(), m_Board->GetInstanceCount(), 
									worldMatrix, viewMatrix, projectionMatrix, m_Board->GetTexture());
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
