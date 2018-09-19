////////////////////////////////////////////////////////////////////////////////
// Filename: instanceshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "instance.ps.h"
#include "instance.vs.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: InstanceShaderClass
////////////////////////////////////////////////////////////////////////////////
class InstanceShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	InstanceShaderClass();
	InstanceShaderClass(const InstanceShaderClass&);
	~InstanceShaderClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int, int);

private:
	ID3D11VertexShader*	m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11InputLayout*	m_layout;
	ID3D11Buffer*		m_matrixBuffer;
};
