#include "LightingAndTexturingShader.hlsl"



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


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSTexture( PS_INPUT input) : SV_Target
{
    float4 lightColor = float4(0,0,0,0);
    for(int index = 0;index != MAX_LIGHTS; index++)
	{
        if(lights[index].typeOfLight == 0.0f)
			lightColor +=  ParellelLight(objectMaterial,lights[index],input.WorldPos,input.Norm,EyePosition);
        if(lights[index].typeOfLight == 1.0f)
			lightColor +=  PointLight(objectMaterial,lights[index],input.WorldPos,input.Norm,EyePosition);
        if(lights[index].typeOfLight == 2.0f)
			lightColor +=  Spotlight(objectMaterial,lights[index],input.WorldPos,input.Norm,EyePosition);
    }
	return  txDiffuse1.Sample(samLinear, input.TexCord) * saturate (lightColor);
}


