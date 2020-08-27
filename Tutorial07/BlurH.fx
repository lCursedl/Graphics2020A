//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D brightRT : register(t0);

SamplerState samp : register( s0 );

cbuffer BlurHCB : register (b0)
{
	int mipLevel;
	float3 fViewportWidth;
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
	
	float width = fViewportWidth.x;
	for (int i = 0; i < mipLevel; i++)
	{
		width /= 2.f;
	}

	float pixelOffset = 1.f / width;

	float2 offset[] =
	{
		float2(-4, 0),
		float2(-3, 0),
		float2(-2, 0),
		float2(-1, 0),
		float2(0, 0),
		float2(1, 0),
		float2(2, 0),
		float2(3, 0),
		float2(4, 0)
	};

	float blurWeights[] =
	{
	   0.004815026f,
	   0.028716039f,
	   0.102818575f,
	   0.221024189f,
	   0.28525234f,
	   0.221024189f,
	   0.102818575f,
	   0.028716039f,
	   0.004815026f
	};

	float3
	blurH =  brightRT.SampleLevel(samp, input.texcoord.xy + (offset[0] * pixelOffset), mipLevel).xyz * blurWeights[0];
	blurH += brightRT.SampleLevel(samp, input.texcoord.xy + (offset[1] * pixelOffset), mipLevel).xyz * blurWeights[1];
	blurH += brightRT.SampleLevel(samp, input.texcoord.xy + (offset[2] * pixelOffset), mipLevel).xyz * blurWeights[2];
	blurH += brightRT.SampleLevel(samp, input.texcoord.xy + (offset[3] * pixelOffset), mipLevel).xyz * blurWeights[3];
	blurH += brightRT.SampleLevel(samp, input.texcoord.xy + (offset[4] * pixelOffset), mipLevel).xyz * blurWeights[4];
	blurH += brightRT.SampleLevel(samp, input.texcoord.xy + (offset[5] * pixelOffset), mipLevel).xyz * blurWeights[5];
	blurH += brightRT.SampleLevel(samp, input.texcoord.xy + (offset[6] * pixelOffset), mipLevel).xyz * blurWeights[6];
	blurH += brightRT.SampleLevel(samp, input.texcoord.xy + (offset[7] * pixelOffset), mipLevel).xyz * blurWeights[7];
	blurH += brightRT.SampleLevel(samp, input.texcoord.xy + (offset[8] * pixelOffset), mipLevel).xyz * blurWeights[8];

	Output.color = float4(blurH.xyz, 1.f);

	return Output;
}