////////////////////////////////////////////////////////////////////////////////
// Filename: triangleclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "drawableinterface.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TriangleClass
////////////////////////////////////////////////////////////////////////////////
class TriangleClass : public DrawableInterface
{
public:
	TriangleClass();
	TriangleClass(const TriangleClass&);
	~TriangleClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext*) override;
};
