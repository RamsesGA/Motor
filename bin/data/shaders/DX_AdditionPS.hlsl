/*
* Shader for DirectX 11.
* Addition
* Made by: Ramses Guerrero Ambriz.
*/

//----------------------------------------------------------------------------
Texture2D txIn0 : register(t0);
Texture2D txIn1 : register(t1);

SamplerState simpleSampler : register(s0);

//----------------------------------------------------------------------------
cbuffer linkToBufferMipLevels : register(b2)
{
  int mipLevle0;    // Bloom clamp
  int mipLevle1;    // Bloom clamp
  int mipLevle2;    // Bloom clamp
  int mipLevle3;    // Bloom clamp
};

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
  float4 Color : SV_Target0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

PS_OUTPUT Add(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;
  output.Color = 0.5f * (txIn0.SampleLevel(simpleSampler, input.Tex, mipLevle0) + txIn1.SampleLevel(simpleSampler, input.Tex, mipLevle1));
  return output;
}