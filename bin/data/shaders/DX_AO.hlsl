/*
* Shader for DirectX 11.
* Made by: Ramses Guerrero Ambriz.
*/

//-----------------------------------------------------------------------------
sampler positionSampler;
sampler normalSampler;

float g_sampleRadius;
float g_intensity;
float g_scale;
float g_bias;

float2 viewportDimensions;

//-----------------------------------------------------------------------------
struct PS_INPUT
{
  float2 texCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 color : COLOR0;
};

//-----------------------------------------------------------------------------
float4 getPosition(in float2 uv)
{
  return tex2D(positionSampler, uv);
}

float3 getNormal(in float2 uv) 
{
  return normalize(tex2D(normalSampler, uv).xyz);
}

float2 getRandom(in float2 uv)
{
  float noiseX = (frac(sin(dot(uv, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
  float noiseY = (frac(sin(dot(uv, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
  float noiseZ = (frac(sin(dot(uv, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));

  return normalize(float3(noiseX, noiseY, noiseZ));
}

float doAmbientOclussion(in float2 tCoord, in float2 uv, in float3 p, in float3 cNorm)
{
  float3 diff = getPosition(tCoord * uv) - p;
  
  const float3 v = normalize(diff);
  const float d = length(diff) * g_scale;

  return max(0.0f, dot(cNorm, v) - g_bias) * (1.0f / (1.0f + d)) * g_intensity;
}

PS_OUTPUT ps_ssao(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;

  output.color = float4(1, 1, 1, 1);

  const float2 vec[4] =
  {
    float2(1, 0), float2(-1, 0)
    float2(0, 1), float2(0, -1),
  };

  float4 p = getPosition(input.texCoord);

  float3 n = getNormal(input.texCoord);
  float2 rand = getRandom(input.texCoord);

  float ao = 0.0f:
  float rad = g_sampleRadius / p.z;

  int iteration = 4;

  for (int i = 0; i < iteration; ++i)
  {
    float2 coord1 = reflect(vec[i], rand) * rad;
    float2 coord2 = float2(coord1.x * 0.707 - coord1.y * 0.707,
                           coord1.x * 0.707 - coord1.y * 0.707);

    ao += doAmbientOclussion(input.texCoord, coord1 * 0.25, p.xyz, n);
    ao += doAmbientOclussion(input.texCoord, coord2 * 0.5,  p.xyz, n);
    ao += doAmbientOclussion(input.texCoord, coord1 * 0.75, p.xyz, n);
    ao += doAmbientOclussion(input.texCoord, coord2,        p.xyz, n);
  }

  ao /= (iteration * 4);

  output.color.rgb = 1 - ao.xxx;

  return output;
}