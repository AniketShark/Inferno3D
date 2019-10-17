#include "LightingAndTexturingShader.fx"

float4 MixTextures(Texture2D texture1,Texture2D texture2,float gammaValue,PS_INPUT input)
{
    float4 color1;
    float4 color2;
    float4 blendColor;
    // Get the pixel color from the first texture.
    color1 = texture1.Sample(samLinear, input.TexCord);
    // Get the pixel color from the second texture.
    color2 = texture2.Sample(samLinear, input.TexCord);
    // Blend the two pixels together and multiply by the gamma value.
    blendColor = color1 * color2 * gammaValue;
    // Saturate the final color.
    blendColor = saturate(blendColor);
    return blendColor;
}

RasterizerState WireframeRS
{
	CullMode = Back;
	FillMode = Wireframe;
	FrontCounterClockwise = false;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSTextureOnly( PS_INPUT input) : SV_Target
{

	float4 color = txDiffuse1.Sample(samLinear, input.TexCord);// + float4(0.3f,0,0,1);
	//if(transperacyOn.x == 1.0f)
		//color = float4(color.xyz,0.3f);
	
	return color;
}
