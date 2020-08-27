//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

#define MAX_BONES 100

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer CBNeverChanges : register( b0 )
{
    matrix mView;
};

cbuffer CBChangeOnResize : register( b1 )
{
    matrix mProjection;
};

cbuffer CBChangesEveryFrame : register( b2 )
{
	matrix mWorld;
	float4 vMeshColor;
	float4 vmViewPos;
};

cbuffer LightDir : register (b3)
{
	float4 mLightDir;
	float3 lightPointPos;
	float4 lightPointAtt;
	float4 lightColor;
};

cbuffer BoneCB : register (b4)
{
	matrix gBones[MAX_BONES];
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 msPos		: POSITION0;
	float2 texcoord		: TEXCOORD0;
	int4 BoneID			: BLENDINDICES0;
	float4  Weights		: BLENDWEIGHT0;
	/*float3 msNormal		: NORMAL0;
	float3 msBinormal	: BINORMAL0;
	float3 msTangent	: TANGENT0;*/
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
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

	matrix BoneTransform = mul(input.Weights.x,gBones[input.BoneID.x]);
	BoneTransform		+= mul(input.Weights.y,gBones[input.BoneID.y]);
	BoneTransform		+= mul(input.Weights.z,gBones[input.BoneID.z]);
	BoneTransform		+= mul(input.Weights.w,gBones[input.BoneID.w]);

	output.Pos = mul(BoneTransform, float4(input.msPos.xyz, 1.0));
	//output.Pos = mul(float4(input.msPos.xyz, 1.0), mWorld);
    output.Pos = mul( output.Pos, mWorld );
    output.Pos = mul( output.Pos, mView );
    output.Pos = mul( output.Pos, mProjection );
    output.Tex = input.texcoord;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT input) : SV_Target
{
	//Light
	//float4 light = normalize(-mLightDir);
	//Dot product
	//float Ndl = dot(input.Normal, light);
	//Output
	float4 color = (txDiffuse.Sample( samLinear, input.Tex ) *vMeshColor);
	return color;
}
