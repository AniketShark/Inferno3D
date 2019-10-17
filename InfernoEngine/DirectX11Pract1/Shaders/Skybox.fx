#include "LightingAndTexturingShader.fx"



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSSkybox( PS_INPUT input) : SV_Target
{
		float4 finalColour = float4(0,0,0,1);
		finalColour = skyBoxTexture.Sample( samLinear , (input.Norm.xyz) );
		return finalColour;
}


