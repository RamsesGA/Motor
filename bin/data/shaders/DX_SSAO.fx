/*
* Shader for DirectX 11.
* Made by: Ramses Guerrero Ambriz.
*/

Texture2D positionSampler : register(t0);
Texture2D normalSampler : register(t1);

SamplerState simpleSampler : register(s0);

//----------------------------------------------------------------------------
cbuffer linkTocbSSAO : register(b2)
{
  float2 mViewportDimensions;
  float2 mNothing;

  float mSample_radius;
  float mIntensity;
  float mScale;
  float mBias;
}

struct PS_INPUT
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 mColor : SV_Target0;
};

//----------------------------------------------------------------------------
float4 GetPosition(in float2 uv)
{
  return positionSampler.Sample(simpleSampler, uv);
}

float3 GetNormal(in float2 uv)
{
  return normalize(normalSampler.Sample(simpleSampler, uv).xyz);
}

float2 GetRandom(in float2 uv)
{
  float noiseX = (frac(sin(dot(uv, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
  float noiseY = (frac(sin(dot(uv, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
  float noiseZ = (frac(sin(dot(uv, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));

  return normalize(float3(noiseX, noiseY, noiseZ));
}

float DoAmbientOcclusion(in float2 tcoord, in float2 uv, in float3 p, in float3 cnorm)
{
  //Saca una coordenada en espacio de mundo.
  float3 diff    = GetPosition(tcoord + uv) - p;
  const float3 v = normalize(diff);
  const float d  = length(diff) * mScale;

  return max(0.0f, dot(cnorm, v) - mBias) * (1.0 / (1.0 + d)) * mIntensity;
}

PS_OUTPUT ps_ssao(PS_INPUT input)
{
  PS_OUTPUT output;
  output.mColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

  const float2 vec[4] =
  {
    float2(1.0f, 0.0f), float2(-1.0f, 0.0f), float2(0.0f, 1.0f), float2(0.0f, -1.0f),
  };

  float4 p = GetPosition(input.texCoord);

  float3 n = GetNormal(input.texCoord);
  float2 randValue = GetRandom(input.texCoord);

  float ao = 0.0f;
  float rad = mSample_radius / p.z;

  int iterators = 4;
  for (int j = 0; j < iterators; ++j)
  {
    float2 coord1 = reflect(vec[j], randValue) * rad;
    float2 coord2 = float2(coord1.x * 0.707 - coord1.y * 0.707,
                           coord1.x * 0.707 + coord1.y * 0.707);

    ao += DoAmbientOcclusion(input.texCoord, coord1 * 0.25, p.xyz, n);
    ao += DoAmbientOcclusion(input.texCoord, coord2 * 0.5,  p.xyz, n);
    ao += DoAmbientOcclusion(input.texCoord, coord1 * 0.75, p.xyz, n);
    ao += DoAmbientOcclusion(input.texCoord, coord2, p.xyz, n);
  }

  ao /= (iterators * 4);
  output.mColor.rgb = 1 - ao.xxx;

  return output;
}