//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D posRT : register(t0);
Texture2D specularRT : register(t1);
Texture2D normalRT : register(t2);
Texture2D albedoRT : register(t3);
Texture2D skyboxRT : register(t4);
TextureCube ambientMap : register(t5);

SamplerState samp : register( s0 );

cbuffer LightCB : register(b0)
{
	float kDiffuse;
	float kAmbient;	
	float kSpecular;
	float specularPower;
	float4 lightDir;
	float4 lightColor;
	float4 vViewPos;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 msPos		: POSITION0;
	float2 texcoord		: TEXCOORD0;
	float3 msNormal		: NORMAL0;
	float3 msBinormal	: BINORMAL0;
	float3 msTangent	: TANGENT0;
};

struct PS_INPUT
{
    float4 psPos		: SV_POSITION;
    float2 texcoord		: TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 color		: SV_TARGET0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vs_main( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	input.msPos.y = mul(input.msPos.y, -1);
	output.psPos = float4(input.msPos, 1.0f);
    output.texcoord = input.texcoord;
    
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT ps_main( PS_INPUT input) : SV_Target
{
	PS_OUTPUT Output;
	
	//Pos sampler
	float4 wsPos = posRT.Sample(samp, input.texcoord).xyzw;
	float mask = wsPos.w;

	//Discard pixel
	//clip(mask < 1.0f ? -1 : 1);
	if (mask > 0.f)
	{
		//Skybox sampler
		float3 skybox = skyboxRT.Sample(samp, input.texcoord);
		Output.color = float4(skybox.xyz, 1.f);
	}
	else
	{
		//Specular sampler
		float specular = specularRT.Sample(samp, input.texcoord).x;

		//Normal sampler
		float3 wsNormal = normalRT.Sample(samp, input.texcoord).xyz;

		//Albedo sampler
		float3 albedo = albedoRT.Sample(samp, input.texcoord).xyz;

		//Ambient color
		float3 wsLightDir = normalize(lightDir.xyz);
		float NdL = clamp(dot(-wsLightDir, wsNormal), 0.0f, 1.0f);
		float3 ambient = pow(ambientMap.Sample(samp, wsNormal), 2.2f);
		float3 ambientColor = ambient * kAmbient * (1.0f - NdL);		

		//Diffuse color
		float3 diffuseColor = (albedo - albedo * specular) * lightColor.xyz * kDiffuse * NdL;

		//Specular color
		float3 wsViewDir = normalize(vViewPos - wsPos);
		float3 wsReflect = reflect(wsLightDir, wsNormal);
		float specularFactor = pow(max(dot(wsViewDir, wsReflect), 0.0f), specularPower);
		float3 specularColor = lerp(0.03f, albedo, specular) * specularFactor * kSpecular * NdL;

		Output.color = float4(ambientColor + diffuseColor + specularColor, 1.0f);
	}

	return Output;
}