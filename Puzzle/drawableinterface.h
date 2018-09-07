////////////////////////////////////////////////////////////////////////////////
// Filename: drawableinterface.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Interface name: DrawableInterface
////////////////////////////////////////////////////////////////////////////////
class DrawableInterface
{
protected:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	DrawableInterface();
	DrawableInterface(const DrawableInterface&);
	~DrawableInterface();

	virtual bool Initialize(ID3D11Device*) = 0;
	virtual void Shutdown() = 0;
	virtual void Render(ID3D11DeviceContext*) = 0;

	int GetIndexCount();

protected:
	bool InitializeBuffers(ID3D11Device*, VertexType*, int, unsigned long*, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *	m_vertexBuffer, *m_indexBuffer;
	int				m_vertexCount, m_indexCount;
};

