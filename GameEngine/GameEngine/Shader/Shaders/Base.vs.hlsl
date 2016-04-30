struct VertIn
{
	float4 Pos  : POSITION;
    float4 Color : COLOR;
};

struct VertOut
{
	float4 Pos  : SV_POSITION;
    float4 Color : COLOR;
};

VertOut main( VertIn vIn )
{
	VertOut vOut;
	
	vOut.Pos = vIn.Pos;
	vOut.Color = vIn.Color;

	return vOut;
}