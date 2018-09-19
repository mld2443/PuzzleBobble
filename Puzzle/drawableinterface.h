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

	struct InstanceType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	DrawableInterface();
	DrawableInterface(const DrawableInterface&);
	~DrawableInterface();

	virtual bool Initialize(ID3D11Device*, ID3D11DeviceContext*) = 0;
	virtual void Shutdown() = 0;
	virtual void Render(ID3D11DeviceContext*) = 0;

	int GetVertexCount();
	int GetIndexCount();
	int GetInstanceCount();

	bool isInstanced();

protected:
	bool InitializeVertexBuffer(ID3D11Device*, VertexType*, int);
	bool InitializeIndexBuffer(ID3D11Device*, unsigned long*, int);
	bool InitializeInstanceBuffer(ID3D11Device*, InstanceType*, int);

	void ShutdownBuffers();
	
	void RenderWithInstanceBuffer(ID3D11DeviceContext*);
	void RenderWithoutInstanceBuffer(ID3D11DeviceContext*);

private:
	ID3D11Buffer *	m_vertexBuffer, *m_indexBuffer, *m_instanceBuffer;
	int				m_vertexCount, m_indexCount, m_instanceCount;
	bool			m_instanced;
};

