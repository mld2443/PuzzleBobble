////////////////////////////////////////////////////////////////////////////////
// Filename: instance.ps.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 HSVA : COLOR;
	float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Color Conversions
////////////////////////////////////////////////////////////////////////////////

// NOTE: Color Functions courtesy of Ian Taylor
// http://www.chilliant.com/rgb2hsv.html

float3 HUEtoRGB(in float H)
{
	float R = abs(H * 6.0f - 3.0f) - 1.0f;
	float G = 2.0f - abs(H * 6.0f - 2.0f);
	float B = 2.0f - abs(H * 6.0f - 4.0f);
	return saturate(float3(R, G, B));
}


float3 HSVtoRGB(in float3 HSV)
{
	float3 RGB = HUEtoRGB(HSV.x);
	return ((RGB - 1) * HSV.y + 1) * HSV.z;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PSMain(PixelInputType input) : SV_TARGET
{
	float4 textureHSVA, outputRGBA;
	float3 tempRGB;


	// Sample the texture using the sampler at this texture coordinate location.
	textureHSVA = shaderTexture.Sample(SampleType, input.tex);

	// Modify the saturation and value of our pixel color using the instance input colors.
	textureHSVA.y *= input.HSVA.y;
	textureHSVA.z *= input.HSVA.z;

	// Calculate the RGB values of our pixel from the input HSV
	tempRGB = HSVtoRGB(float3(input.HSVA.x, textureHSVA.yz));

	// Combine the RGB we just got with our alpha channel to get the final color.
	outputRGBA = float4(tempRGB, textureHSVA.w * input.HSVA.w);

	return outputRGBA;
}
