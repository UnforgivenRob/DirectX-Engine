struct VertIn
{
	float3 Pos  : POSITION;
    float2 uv :	UV;
	float3 norm : NORMAL;
};

struct VertOut
{
	float4 Pos  : SV_POSITION;
    float4 Color : COLOR;
};

VertOut main( VertIn vIn )
{
	VertOut vOut;
	
	vOut.Pos = float4(vIn.Pos, 1.0f);
	vOut.Color = float4( 1.0f, .5f, 0.0f, 1.0f);

	return vOut;
}