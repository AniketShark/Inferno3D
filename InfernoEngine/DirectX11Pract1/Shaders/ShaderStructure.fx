

struct Material
{
	float4 diffuse;
	float4 ambient;
	float4 specular;
	float4 meshColor;
	
};

struct LightInfo
{
	float4 vPosition;
	float4 vLightDir;
	float4 attenuation;
	float power;	
	float range;
	float typeOfLight;
	float padding;
	Material material;
};

float4 PointLight(Material objectMaterial,LightInfo lightInfo,float4 VPosition,float4 VNormal,float4 eyePos)
{
	
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	VNormal = normalize( VNormal ); // Error or sanity check...

	// The vector from the surface to the light.
	float3 lightPos = lightInfo.vPosition.xyz;
	float3 vertexPos = VPosition.xyz;
	float3 lightVec = lightPos - vertexPos;

	// The distance from surface to light.
	float d = length(lightVec);

	if( d > lightInfo.range )		// y element is range of spot or point light
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);		// If it's too far, set colour to black. 
		//Note: since it's black, we don't need to to any more calculations.
	}

	// Normalize the light vector.
	lightVec /= d;			//normalize( lightVec );

	// Add the ambient light term.
	// 	litColor += v.diffuse.xyz * L.material.ambient.xyz;
	litColor +=  objectMaterial.diffuse.rgb * lightInfo.material.ambient.rgb;			// Same as above: xyzw same as rgba

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, VNormal.xyz);

	[branch]
	if( diffuseFactor > 0.0f )	// +ve so it's the front of the object
	{
		float specPower  = max(objectMaterial.specular.a, 1.0f);
		float3 toEye     = normalize( eyePos.xyz - VPosition.xyz);
		float3 R         = reflect( -lightVec, VPosition.xyz);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		// diffuse and specular terms
		litColor += diffuseFactor * objectMaterial.diffuse.rgb * lightInfo.material.diffuse.rgb;
		litColor += specFactor * objectMaterial.specular.rgb * lightInfo.material.diffuse.rgb;
	}
	
	//float attenuation = 1 / (lightInfo.attenuation.x*d*d + lightInfo.attenuation.y * d + lightInfo.attenuation.z) ;

	 //attenuation =  saturate( attenuation );

	// litColor *= attenuation;
	// attenuate
	litColor =  litColor / dot(lightInfo.attenuation.rgb, float3(1.0f, d, d*d));

	return float4( litColor.xyz, 1.0f );
}

float4 ParellelLight(Material objectMaterial,LightInfo lightInfo,float4 VPosition,float4 VNormal,float4 eyePos)
{
	
	VNormal = normalize( VNormal );

	float3 litColor = float3(0.0f, 0.0f, 0.0f);
 
	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -lightInfo.vLightDir.xyz;
	
	// Add the ambient term.
	litColor += objectMaterial.diffuse.rgb * lightInfo.material.ambient.rgb;	
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	float diffuseFactor = dot( lightVec, VNormal.xyz );

	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower  = max(objectMaterial.specular.a, 1.0f);
		float3 toEye     = normalize( eyePos.xyz - VPosition.xyz );
		float3 R         = reflect( lightVec, VNormal.xyz );
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
		
		// diffuse and specular terms
		litColor += diffuseFactor * objectMaterial.diffuse.rgb * lightInfo.material.diffuse.rgb;
		//litColor += specFactor * objectMaterial.specular.rgb * lightInfo.material.diffuse.rgb;
	}
	return  float4( litColor.rgb, 1.0f );
}



//float3 Spotlight(SurfaceInfo v, Light L, float3 eyePos)
float4 Spotlight(Material objectMaterial,LightInfo lightInfo,float4 VPosition,float4 VNormal,float4 eyePos)
{
	VNormal = normalize( VNormal );
	
	float4 litColor = PointLight(objectMaterial, lightInfo, VPosition, VNormal, eyePos);
			
	// The vector from the surface to the light.
	float3 lightVec = normalize(lightInfo.vPosition.xyz - VPosition.xyz);
	
	float s = pow(max(dot(-lightVec, lightInfo.vLightDir.xyz), 0.0f), lightInfo.power);
	
	// Scale color by spotlight factor.
	return litColor * s;
}