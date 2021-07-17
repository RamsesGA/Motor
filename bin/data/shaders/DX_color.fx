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

//----------------------------------------------------------------------------
struct VS_INPUT
{
  float4 position  : POSITION0;
  float4 normal    : NORMAL0;
  float4 tangent   : TANGENT0;
  float2 texCoords : TEXCOORD0;
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
  PS_INPUT Output = (PS_INPUT)0;

  Output.position = mul(input.position, mWorld);
  Output.position = mul(Output.position, mView);
  Output.position = mul(Output.position, mProjection);
  Output.texCoords = input.texCoords.xy;

  return (Output);
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