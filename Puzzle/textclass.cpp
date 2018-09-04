////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textclass.h"



TextClass::TextClass()
{
	m_brush = nullptr;
	m_format = nullptr;
	m_layout = nullptr;
}


TextClass::TextClass(const TextClass&)
{
}


TextClass::~TextClass()
{
}


bool TextClass::Initialize(IDWriteFactory* dwriteFactory, ID2D1DeviceContext* deviceContext, WCHAR* text)
{
	HRESULT result;


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

	// Create the text layout for our sample text.
	result = dwriteFactory->CreateTextLayout(text, static_cast<UINT32>(sizeof(text)),
		m_format, 4096.0f, 4096.0f, &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void TextClass::Shutdown()
{
	// Release the layout object.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	// Release the layout object.
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
	deviceContext->DrawTextLayout(D2D1::Point2F(0.0f, 0.0f), m_layout, m_brush);
	deviceContext->EndDraw();
	return;
}