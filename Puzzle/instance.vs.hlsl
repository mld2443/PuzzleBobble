////////////////////////////////////////////////////////////////////////////////
// Filename: instance.vs.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : POSITION0;
	float4 color : COLOR0;
	float4 instancePosition : POSITION1;
	float4 instanceColor: COLOR1;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VSMain(VertexInputType input)
{
	PixelInputType output;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Update the position of the vertices based on the data for this particular instance.
	input.position.x += input.instancePosition.x;
	input.position.y += input.instancePosition.y;
	input.position.z += input.instancePosition.z;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the input color for the pixel shader to use.
	output.color.x = input.color.x * input.instanceColor.x;
	output.color.y = input.color.y * input.instanceColor.y;
	output.color.z = input.color.z * input.instanceColor.z;
	output.color.w = input.color.w * input.instanceColor.w;

	return output;
}