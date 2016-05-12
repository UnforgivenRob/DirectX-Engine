struct VertIn
{
	float3 Pos  : POSITION;
    float2 uv :	UV;
	float3 norm : NORMAL;
};

cbuffer VS_CONSTANT_BUFFER
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

struct VertOut
{
	float4 Pos  : SV_POSITION;
    float4 Color : COLOR;
};

VertOut main( VertIn vIn )
{
	VertOut vOut;
	
	vOut.Pos = mul( proj, mul( view, mul( model, float4( vIn.Pos, 1.0f ) ) ) );
	vOut.Color = float4( vIn.Pos, 1.0f);

	return vOut;
}