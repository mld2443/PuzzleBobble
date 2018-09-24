////////////////////////////////////////////////////////////////////////////////
// Filename: shaderinterface.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Interface name: ShaderInterface
////////////////////////////////////////////////////////////////////////////////
class ShaderInterface
{
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ShaderInterface();
	ShaderInterface(const ShaderInterface&);
	~ShaderInterface();

	virtual bool Initialize(ID3D11Device*) = 0;
	virtual void Shutdown() = 0;
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

protected:
	bool InitializeShader(ID3D11Device*, const BYTE*, SIZE_T, const BYTE*, SIZE_T, D3D11_INPUT_ELEMENT_DESC*, unsigned int);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	virtual void DrawShader(ID3D11DeviceContext*, int, int) = 0;

private:
	ID3D11VertexShader*	m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11InputLayout*	m_layout;
	ID3D11Buffer*		m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};
