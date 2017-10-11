
///////////////////////////////////
// shared vertex format
///////////////////////////////////
struct VERTEX_UNLIT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VERTEX_LIT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};