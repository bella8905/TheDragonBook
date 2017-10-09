#include "vertex.fxh"

cbuffer ConstantBuffer : register( b0 )
{
    matrix model;
    matrix view;
    matrix projection;

    bool bSelected;
}

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

// vertex shader
VS_OUTPUT VS( VERTEX input )
{
    VS_OUTPUT output = ( VS_OUTPUT )0;
    output.pos = mul( model, float4( input.position, 1.f ) );
    output.pos = mul( view, output.pos );
    output.pos = mul( projection, output.pos );
    output.color = input.color;
    return output;
}

// pixel shader
float4 PS( VS_OUTPUT input ) : SV_TARGET
{
    return input.color;
}

// technique
technique11 UnlitTech
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}