/*
* Shader for DirectX 11.
* Geometry buffer.
* Made by: Ramses Guerrero Ambriz.
*/

//----------------------------------------------------------------------------
Texture2D baseColor : register(t0);
Texture2D metallic : register(t1);
Texture2D roughness : register(t2);
Texture2D normal : register(t3);
Texture2D emissive : register(t4);
Texture2D opacity : register(t5);

SamplerState simpleSampler : register(s0);

//----------------------------------------------------------------------------
cbuffer linkToBufferCamera : register(b0)
{
  matrix mView;
  matrix mProjection;
}

cbuffer linkToBufferWorld : register(b1)
{
  matrix mWorld;
  float4 objectPosition;
}

cbuffer linkToBufferModelData : register(b2)
{
  matrix modelMatrix;
  matrix bonesTransform[200];
}


//----------------------------------------------------------------------------
struct VS_INPUT
{
  float4 position    : POSITION0;
  float3 normal      : NORMAL0;
  float3 tangent     : TANGENT0;
  float3 biTangent   : BINORMAL0;
  float2 texCoords   : TEXCOORD0;
  float4 boneWeights : BLENDWEIGHT0;
  uint4  boneIds      : BLENDINDICES0;
};

struct PS_INPUT
{
  float4 position  : SV_POSITION;
  float3 posView   : TEXCOORD0;
  float2 texCoords : TEXCOORD1;
  float3x3 TBN     : TEXCOORD2;
};

struct PS_OUTPUT 
{
  float4 diffColor     : COLOR0;
  float4 normal        : COLOR1;
  float4 position      : COLOR2;
  float4 emissiveColor : COLOR3;
};

//----------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------
PS_INPUT vs_gBuffer(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;

  matrix newMatWorld;
  newMatWorld[0] = float4(1, 0, 0, 0);
  newMatWorld[1] = float4(0, 1, 0, 0);
  newMatWorld[2] = float4(0, 0, 1, 0);
  newMatWorld[3] = objectPosition;

  matrix matWV = mul(newMatWorld, modelMatrix);
  matWV = mul(matWV, mView);

  output.position = mul(input.position, matWV);
  output.posView = output.position.xyz;

  output.position = mul(output.position, mProjection);

  output.texCoords = input.texCoords;

  output.TBN[0] = normalize(mul(float4(input.tangent, 0.0f), matWV)).xyz;
  output.TBN[2] = normalize(mul(float4(input.normal, 0.0f), matWV)).xyz;
  output.TBN[1] = normalize(cross(output.TBN[2], output.TBN[0])).xyz;

  return(output);
}

//----------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------
PS_OUTPUT ps_gBuffer(PS_INPUT input) : SV_Target
{
  PS_OUTPUT output = (PS_OUTPUT)0;

  output.position          = float4(input.posView, 1.0f);

  output.normal.xyz        = 2.0f * normal.Sample(simpleSampler, input.texCoords) - 1.0f;
  output.normal.xyz        = normalize(mul(output.normal.xyz, input.TBN));

  output.normal.w          = roughness.Sample(simpleSampler, input.texCoords).x;
  output.diffColor.xyz     = baseColor.Sample(simpleSampler, input.texCoords).xyz;
  output.diffColor.w       = metallic.Sample(simpleSampler, input.texCoords).x;
  output.emissiveColor.xyz = emissive.Sample(simpleSampler, input.texCoords).xyz;

  return (output);
}