
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix model;
	matrix view;
	matrix projection;

	bool bSelected;
}

struct VS_IN
{
	float3 position : POSITION;
	// float3 normal : NORMAL;
	float4 color : COLOR;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT ColorVertexShader( VS_IN input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul( model, float4( input.position, 1.f ) );
	output.pos = mul( view,  output.pos );
	output.pos = mul( projection, output.pos );
	output.color = input.color;
	return output;
}
