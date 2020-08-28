//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D outputRT : register(t0);
Texture2D ssaoRT : register(t1);
Texture2D addBrightRT : register(t2);

SamplerState samp : register( s0 );

cbuffer ToneMapCB : register (b0)
{
	float kExposure;
	float kBloom;
	int colorSpace;
	float filler;
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


/*
*	Shader Function
*/

float3 BasicExposure(float3 Color)
{
	Color *= kExposure;
	Color = pow(Color, 1.0f / 2.2f);

	return Color;
}

float3 Reinhard(float3 Color)
{
	Color *= kExposure;
	Color /= (1.0f + Color);
	Color = pow(Color, 1.0f / 2.2f);

	return Color;
}

float3 BurgessDawson(float3 Color)
{
	Color *= kExposure;
	float3 x = max(0, Color - 0.004f);
	Color = (x * (6.2f * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);

	return Color;
}

float3 Uncharted2Tonemap(float3 x)
{
	float A = 0.15f;
	float B = 0.5f;
	float C = 0.1f;
	float D = 0.2f;
	float E = 0.02f;
	float F = 0.3f;

	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float3 Uncharted2(float3 Color)
{
	Color *= kExposure;
	float W = 11.2f;
	Color = Uncharted2Tonemap(Color);
	float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
	Color *= whiteScale;

	Color = pow(Color, 1.0f / 2.2f);
	return Color;
}

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
	
	//Output color sample
	float ssao = ssaoRT.Sample(samp, input.texcoord).x;
	float3 color = outputRT.Sample(samp, input.texcoord).xyz;
	float3 bloom = addBrightRT.Sample(samp, input.texcoord).xyz * kBloom;

	color *= ssao;

	//Color space fn
	if (colorSpace == 0)
	{
		color = BasicExposure(color);
		bloom = BasicExposure(bloom);
	}
	else if (colorSpace == 1)
	{
		color = Reinhard(color);
		bloom = Reinhard(bloom);
	}
	else if (colorSpace == 2)
	{
		color = BurgessDawson(color);
		bloom = BurgessDawson(bloom);
	}
	else if (colorSpace == 3)
	{
		color = Uncharted2(color);
		bloom = Uncharted2(bloom);
	}

	color += bloom.xyz;

	Output.color = float4(color, 1.f);

	return Output;
}