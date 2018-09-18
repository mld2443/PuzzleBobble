////////////////////////////////////////////////////////////////////////////////
// Filename: boardclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "drawableinterface.h"


///////////////
// CONSTANTS //
///////////////
#define SQRT075 0.86602540378f


////////////////////////////////////////////////////////////////////////////////
// Class name: BoardClass
////////////////////////////////////////////////////////////////////////////////
class BoardClass : public DrawableInterface
{
public:
	struct InstanceType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	BoardClass();
	BoardClass(const BoardClass&);
	~BoardClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext*) override;

	int GetInstanceCount();

private:
	bool InitializeInstanceBuffer(ID3D11Device*);
	void ShutdownInstanceBuffer();
	void RenderWithInstanceBuffer(ID3D11DeviceContext*);

private:
	ID3D11Buffer *	m_instanceBuffer;
	int				m_instanceCount;
};

