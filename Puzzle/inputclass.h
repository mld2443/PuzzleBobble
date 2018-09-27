////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <map>
#include <functional>
#include <dinput.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	void AddKeydownCallback(unsigned int, std::function<void()>);

	bool IsKeyDown(unsigned int);
	void GetMouseLocation(int&, int&);
	bool IsLeftMouseButtonDown();

private:
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8*			m_directInput;
	IDirectInputDevice8*	m_mouse;

	bool			m_keys[256];
	DIMOUSESTATE	m_mouseState;

	std::map<unsigned int, std::function<void()>> m_keydownCallbacks;

	int	m_screenWidth, m_screenHeight;
	int	m_mouseX, m_mouseY;
};
