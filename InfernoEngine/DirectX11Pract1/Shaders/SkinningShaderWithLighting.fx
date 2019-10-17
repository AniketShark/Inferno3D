

#include "LightingAndTexturingShader.fx"


PS_INPUT VSSkinning( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	 
	matrix matFinalMVP = mul(World,View);
    matFinalMVP = mul(matFinalMVP, Projection);

	output.Pos = input.Pos;
	output.Color = input.Color;

	if(GpuSkinning == 1)
	{
		output.Color = float4(1,0,0,1);
		uint boneIndex = input.BoneIndex.x;
        float boneWeight = 1;//input.BoneWeight.x;
        matrix k = boneMatrices[boneIndex];
        output.Pos = boneWeight * mul(output.Pos, k);
    }
	
	output.Pos = mul( output.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );

	
	
    return output;
}



