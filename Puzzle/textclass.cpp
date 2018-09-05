////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textclass.h"



TextClass::TextClass()
{
	m_brush = nullptr;
	m_format = nullptr;
}


TextClass::TextClass(const TextClass&)
{
}


TextClass::~TextClass()
{
}


bool TextClass::Initialize(IDWriteFactory* dwriteFactory, ID2D1DeviceContext* deviceContext, const std::wstring& text)
{
	HRESULT result;


	// Initialize drawing window to (0, 0, 200, 200)
	SetDrawWindow(200, 200, 800, 800);

	// Initialize text string to passed in text
	SetTextString(text);

	// Create our solid white brush.
	result = deviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_brush);
	if (FAILED(result))
	{
		return false;
	}

	// Create our text format with specified parameters.
	result = dwriteFactory->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 36.0f, L"en-US", &m_format);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void TextClass::Shutdown()
{

	// Release the format object.
	if (m_format)
	{
		m_format->Release();
		m_format = nullptr;
	}

	// Release the brush object.
	if (m_brush)
	{
		m_brush->Release();
		m_brush = nullptr;
	}

	return;
}


void TextClass::Render(ID2D1DeviceContext* deviceContext)
{
	deviceContext->BeginDraw();
	deviceContext->DrawTextW(m_string.c_str(), (UINT32)m_string.size(), m_format, m_drawWindow, m_brush);
	deviceContext->EndDraw();
	return;
}


void TextClass::SetDrawWindow(float xLeft, float yTop, float xRight, float yBottom)
{
	m_drawWindow = D2D1::RectF(xLeft, yTop, xRight, yBottom);
}


void TextClass::SetTextString(const std::wstring& text)
{
	m_string = text;
}

