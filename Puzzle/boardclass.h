////////////////////////////////////////////////////////////////////////////////
// Filename: boardclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "drawableinterface.h"
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: BoardClass
////////////////////////////////////////////////////////////////////////////////
class BoardClass : public DrawableInterface
{
public:
	BoardClass();
	BoardClass(const BoardClass&);
	~BoardClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext*) override;

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);

private:
	TextureClass* m_Texture;
};

