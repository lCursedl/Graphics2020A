//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
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
    float4 vMeshColor;
};

cbuffer LightCB : register (b3)
{
	float4 mLightDir;
	float3 lightPointPos;
	float4 lightPointAtt;
	float4 lightColor;
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
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL0;
};

struct PS_OUTPUT
{
	float4 color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vs_main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.msPos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
	output.Normal = mul(input.msNormal, World);
    output.Tex = input.texcoord;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT input) : SV_Target
{
	//Light
	float4 light = normalize(-mLightDir);
	//Dot product
	float Ndl = dot(input.Normal, light);
	//Output
	float4 color;
	color = (txDiffuse.Sample( samLinear, input.Tex ) *vMeshColor) * Ndl;
	return color;
}
