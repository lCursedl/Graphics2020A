//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
TextureCube SkyboxMap : register( t0 );

SamplerState samp : register( s0 );

cbuffer cbNeverChanges : register(b0)
{
	matrix View;
};

cbuffer cbChangeOnResize : register(b1)
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b2)
{
	matrix World;
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
    float3 wsNormal		: NORMAL0;
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
	//World space position
	float4 wsPos = mul(float4(input.msPos.xyz, 1.0f), World);

	//View space position
	float4 vsPos = mul(wsPos.xyzw, View);

	//Projection space position
	float4 psPos = mul(vsPos.xyzw, Projection);

	//World space normal
	float3 wsNormal = normalize(mul(float4(input.msNormal.xyz, 0.f), World));

	PS_INPUT output = (PS_INPUT)0;
	output.psPos = psPos;
    output.wsNormal = wsNormal;
    
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT ps_main( PS_INPUT input) : SV_Target
{
	PS_OUTPUT Output;
	
	//Ambient color
	float3 albedoColor = pow(SkyboxMap.Sample(samp, input.wsNormal), 2.2f);
	Output.color = float4(albedoColor, 1.0f);

	return Output;
}