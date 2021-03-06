////////////////////////////////////////////////////////////////////////////////
// Filename: instanceshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "shaderinterface.h"
#include "instance.ps.h"
#include "instance.vs.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: InstanceShaderClass
////////////////////////////////////////////////////////////////////////////////
class InstanceShaderClass : public ShaderInterface
{
public:
	InstanceShaderClass();
	InstanceShaderClass(const InstanceShaderClass&);
	~InstanceShaderClass();

	bool Initialize(ID3D11Device*) override;
	void Shutdown() override;

protected:
	void DrawShader(ID3D11DeviceContext*, int, int) override;
};
