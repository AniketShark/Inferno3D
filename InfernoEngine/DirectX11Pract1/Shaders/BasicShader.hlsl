
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

RasterizerState WireFrameRS
{
	FillMode = WireFrame;
	CullMode = Back;
	FrontCounterClockwise = false;
	// Default values used for any properties we do not set.
};


cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
	float4 Color : COLOR0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 WorldPos : POSITION;
	float4 Color : COLOR0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	matrix matFinalMVP = mul(World,View);
    matFinalMVP = mul( matFinalMVP, Projection );
	output.Pos = mul(input.Pos,matFinalMVP);
	output.WorldPos = mul( input.Pos, World );
    output.Color = input.Color;
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	return float4(1,1,1,1);
}



