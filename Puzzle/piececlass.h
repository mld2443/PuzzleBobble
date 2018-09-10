////////////////////////////////////////////////////////////////////////////////
// Filename: piececlass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "drawableinterface.h"
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: PieceClass
////////////////////////////////////////////////////////////////////////////////
class PieceClass : public DrawableInterface
{
public:
	PieceClass();
	PieceClass(const PieceClass&);
	~PieceClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext*) override;

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);

private:
	TextureClass* m_Texture;
};

