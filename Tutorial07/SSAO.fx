//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D posRT : register( t0 );
Texture2D normalRT : register ( t1 );

SamplerState samp : register( s0 );

cbuffer SSAOCB : register (b0)
{
	float kIntensity;
	float kScale;
	float kBias;
	float kSample;
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
};

struct PS_OUTPUT
{
	float4 color		: SV_TARGET0;
};


/*
*	Shader fn
*/

float3 GetPosition(float2 uv)
{
	return posRT.Sample(samp, uv).xyz;
}

float3 GetNormal(float2 uv)
{
	return normalRT.Sample(samp, uv).xyz;
}

float3 GetRandom(float2 uv)
{
	float noiseX = frac(sin(dot(uv, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f);
	float noiseY = frac(sin(dot(uv, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f);
	float noiseZ = frac(sin(dot(uv, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f);

	return normalize(float3(noiseX, noiseY, noiseZ));
}

float DoAmbientOcclusion(float2 texCoord, float2 uv, float3 pos, float3 normal)
{
	float3 diff = GetPosition(texCoord + uv) - pos;
	const float3 v = normalize(diff);
	const float d = length(diff) * kScale;

	return max(0.0f, dot(normal, v) - kBias) * (1.0 / (1.0 + d)) * kIntensity;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vs_main( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	input.msPos.y = mul(input.msPos.y, -1);
	output.psPos = float4(input.msPos, 1.0f);
    output.Tex = input.texcoord;
    
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT ps_main( PS_INPUT input) : SV_Target
{
	PS_OUTPUT Output;
	
	//Sampler pos
	const float2 vec[4] =
	{
		float2(1, 0), float2(-1, 0), float2(0, 1), float2(0, -1)
	};

	//Current pos
	float3 wsPos = GetPosition(input.Tex);

	//Current normal
	float3 wsNormal = GetNormal(input.Tex);

	//Current random
	float3 rand = GetRandom(input.Tex);

	//Output AO
	float AO = 0.0f;

	//Sampler radius
	float rad = kSample / wsPos.z;

	//Sampler addition
	int iterations = 4;
	
	for (int i = 0; i < iterations; i++)
	{
		float2 coord1 = reflect(vec[i], rand) * rad;
		float2 coord2 = float2(coord1.x * 0.707f - coord1.y * 0.707f,
			coord1.x * 0.707f + coord1.y * 0.707f);

		AO += DoAmbientOcclusion(input.Tex, coord1 * 0.25f, wsPos.xyz, wsNormal.xyz);
		AO += DoAmbientOcclusion(input.Tex, coord2 * 0.5f, wsPos.xyz, wsNormal.xyz);
		AO += DoAmbientOcclusion(input.Tex, coord1 * 0.75f, wsPos.xyz, wsNormal.xyz);
		AO += DoAmbientOcclusion(input.Tex, coord2, wsPos.xyz, wsNormal.xyz);
	}

	AO /= (iterations * 4);
	
	Output.color = float4(1.0f - AO.xxx, 1.0f);

	return Output;
}