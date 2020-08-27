//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D albedoMap : register( t0 );
Texture2D normalMap : register ( t1 );
Texture2D specularMap : register( t2 );

SamplerState samLinear : register( s0 );

cbuffer cbNeverChanges : register( b0 )
{
    matrix View;
};

cbuffer cbChangeOnResize : register( b1 )
{
    matrix Projection;
};

cbuffer cbChangesEveryFrame : register( b2 )
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
    float2 Tex			: TEXCOORD0;
	float3 wsPos		: TEXCOORD1;
	float3x3 matTBN		: TEXCOORD2;
};

struct PS_OUTPUT
{
	float4 Pos			: SV_TARGET0;
	float4 Specular		: SV_TARGET1;
	float4 Normal		: SV_TARGET2;
	float4 Albedo		: SV_TARGET3;
	float4 PosLight		: SV_TARGET4;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vs_main( VS_INPUT input )
{    
	//World space pos
    float4 wsPos = mul( float4(input.msPos, 1.0f), World );
	//View space pos
    float4 vsPos = mul( wsPos, View );
	//Projection space pos
    float4 psPos = mul( vsPos, Projection );
	//World space normal
	float3 wsNormal = normalize(mul(float4(input.msNormal.xyz, 0.0f), World).xyz);
	//World space binormal
	float3 wsBinormal = normalize(mul(float4(input.msBinormal.xyz, 0.0f), World).xyz);
	//World space tangent
	float3 wsTangent = normalize(mul(float4(input.msTangent.xyz, 0.0f), World).xyz);

	PS_INPUT output = (PS_INPUT)0;

	output.psPos = psPos;
    output.Tex = input.texcoord;
	output.wsPos = wsPos;
	output.matTBN = float3x3(wsTangent, wsBinormal, wsNormal);
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT ps_main( PS_INPUT input) : SV_Target
{
	PS_OUTPUT Output;
	
	//Position Output
	Output.Pos = float4(input.wsPos.xyz, 1.0f);

	//Specular output
	float3 specular = pow(specularMap.Sample(samLinear, input.Tex.xy).xyz, 2.2f);
	Output.Specular = float4(specular.xyz, 1.0f);

	//Normal output
	float3 normal = normalMap.Sample(samLinear, input.Tex.xy).xyz;
	normal = (normal * 2.0f) - 1.0f;
	normal = normalize(mul(normal, input.matTBN).xyz);
	Output.Normal = float4(normal, 1.0f);

	//Albedo Output
	float3 albedo = pow(albedoMap.Sample(samLinear, input.Tex.xy).xyz, 2.2f);
	Output.Albedo = float4(albedo, 1.0f);

	//Position Light Output
	Output.PosLight = float4(input.wsPos.xyz, 0.0f);

	return Output;
}