//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D luminanceRT : register(t0);
Texture2D outputRT : register(t1);

SamplerState samp : register( s0 );

cbuffer BrightCB : register (b0)
{
	int mipLevel;
	float3 Threshold;
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
	
	float bright = luminanceRT.SampleLevel(samp, input.texcoord, mipLevel).x;
	bright -= Threshold.x;
	bright = max(bright, 0.f);
	float3 colorBright = bright * outputRT.Sample(samp, input.texcoord).xyz;

	Output.color = float4(colorBright, 1.f);

	return Output;
}