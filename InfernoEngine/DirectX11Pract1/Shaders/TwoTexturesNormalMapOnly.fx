#include "LightingAndTexturingShader.fx"



PS_INPUT VSNormalMap(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	matrix matFinalMVP = mul(World,View);
    matFinalMVP = mul( matFinalMVP, Projection );
	output.Pos = mul(input.Pos,matFinalMVP);
	output.WorldPos = mul( input.Pos, World );
	output.Norm = mul(input.Norm,World);
    output.Norm = mul( input.Norm, InverseTransposedWorld );
    output.Color = input.Color;
	output.TexCord = input.Texcoord;
	output.Tangent = input.Tangent;
	output.Binormal = input.Binormal;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//-------------------------------------------------------------------------------------

float4 PSNormalMap( PS_INPUT input ) : SV_Target
{
  
	float4 textureColor;
    float4 bumpMapColor;
    float4 bumpNormal;
    float4 color;
	float lightIntensity;
 // Sample the texture pixel at this location.
    textureColor = txDiffuse1.Sample(samLinear, input.TexCord);
    // Sample the pixel in the bump map.
    bumpMapColor = bumpMap.Sample(samLinear, input.TexCord);
    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMapColor = (bumpMapColor * 2.0f) - 1.0f;
    // Calculate the normal from the data in the bump map.
    bumpNormal = input.Norm  + bumpMapColor.x * input.Tangent + bumpMapColor.y * input.Binormal;
    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);
    //// Invert the light direction for calculations.
	//float4 lightDir = -lights[0].vLightDir;

	 
	float4 lightColor = float4(0,0,0,0);
    for(int index = 0;index != MAX_LIGHTS; index++)
	{
        if(lights[index].typeOfLight == 0.0f)
		lightColor +=  ParellelLight(objectMaterial,lights[index],input.WorldPos,bumpNormal,EyePosition);
        if(lights[index].typeOfLight == 1.0f)
		lightColor +=  PointLight(objectMaterial,lights[index],input.WorldPos,bumpNormal,EyePosition);
    }

    //// Calculate the amount of light on this pixel based on the bump map normal value.
    //lightIntensity = saturate(dot(bumpNormal, lightDir));

    //// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
    //color = saturate( objectMaterial.diffuse * lightIntensity);

    // Combine the final bump light color with the texture color.
    color = lightColor * textureColor;

    return color;
}




technique11 NormalMapping
{
    pass P0
	{
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetPixelShader( CompileShader( ps_4_0, PSNormalMap()));
		//SetRasterizerState(WireFrameRS);
    }

}