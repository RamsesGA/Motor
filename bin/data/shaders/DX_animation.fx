/*
* Shader for DirectX 11.
* Made by: Ramses Guerrero Ambriz.
*/

//----------------------------------------------------------------------------
Texture2D simpleTexture : register(t0);
SamplerState simpleSampler : register(s0);

//----------------------------------------------------------------------------
cbuffer linkToBuffer1 : register(b0)
{
  matrix mView;
  matrix mProjection;
}

cbuffer linkToBuffer2 : register(b1)
{
  matrix mWorld;
  float4 vMeshColor;
}

cbuffer linkToBufferBones : register(b2)
{
  matrix bonesTransform[200];
}

//----------------------------------------------------------------------------
struct VS_INPUT
{
  float4 position    : POSITION0;
  float4 normal      : NORMAL0;
  float4 tangent     : TANGENT0;
  float2 texCoords   : TEXCOORD0;
  float4 boneWeights : BLENDWEIGHT0;
  uint4  boneIds      : BLENDINDICES0;
};

struct PS_INPUT
{
  float4 position  : SV_POSITION;
  float2 texCoords : TEXCOORD0;
};

//----------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;
  float3x3 TBN;

  matrix boneTrans = bonesTransform[input.boneIds[0]] * input.boneWeights[0];

  boneTrans += bonesTransform[input.boneIds[1]] * input.boneWeights[1];
  boneTrans += bonesTransform[input.boneIds[2]] * input.boneWeights[2];
  boneTrans += bonesTransform[input.boneIds[3]] * input.boneWeights[3];
  
  float4 position = mul(input.position, boneTrans);

  output.position = mul(position, mWorld);
  output.position = mul(output.position, mView);
  output.position = mul(output.position, mProjection);

  matrix boneWV = mul(mul(boneTrans, mWorld), mView);

  //tangent
  TBN[0] = normalize(mul(input.tangent, boneWV)).xyz;

  //binormal
  TBN[1] = normalize(cross(TBN[2], TBN[0]));
  
  //normal
  TBN[2] = normalize(mul(input.normal, boneWV)).xyz;
  
  output.texCoords = input.texCoords;

  return output;
}

//----------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    float4 color;
    color = simpleTexture.Sample(simpleSampler, input.texCoords);
    return color;
}