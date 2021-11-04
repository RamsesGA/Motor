/*
* Shader for DirectX 11.
* Compute shader SSAO
* Made by: Ramses Guerrero Ambriz.
*/

//Input data
//----------------------------------------------------------------------------
Texture2D<float4> positionTexture : register(t0);
Texture2D<float4> normalTexture : register(t1);

SamplerState simpleSampler : register(s0);

//Constant buffers
//----------------------------------------------------------------------------
cbuffer linkToBufferSSAO : register(b0)
{
  float2 mViewportDimensions;
  float2 mNothing;
}

cbuffer linkToBufferSSAO2 : register(b1)
{
  float mSample_radius;
  float mIntensity;
  float mScale;
  float mBias;
}

//Compute
//----------------------------------------------------------------------------
RWTexture2D<float4> bufferOut : register(u0);

//Functions
//----------------------------------------------------------------------------
float4 GetPosition(in float2 uv)
{
  return positionTexture.SampleLevel(simpleSampler, uv, 0);
}

float3 GetNormal(in float2 uv)
{
  return normalize(normalTexture.SampleLevel(simpleSampler, uv, 0));
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


//MAIN
//----------------------------------------------------------------------------
[numthreads(32, 32, 1)]
void cs_SSAO(uint3 groupID          : SV_GroupID, 
             uint3 groupThreadID    : SV_GroupThreadID,
			 uint groupIndex        : SV_GroupIndex,
			 uint3 dispatchThreadID : SV_DispatchThreadID){

  //SV_DispatchThreadID = lectura del buffer y escribir
  uint2 index = dispatchThreadID.xy;
  float2 localTexCoords = (dispatchThreadID.xy / mViewportDimensions);
  
  bufferOut[index.xy] = float4(1.0f, 1.0f, 1.0f, 1.0f);

  const float2 vec[4] =
  {
    float2(1.0f, 0.0f), float2(-1.0f, 0.0f), float2(0.0f, 1.0f), float2(0.0f, -1.0f),
  };

  float4 p = GetPosition(localTexCoords);
  float3 n = GetNormal(localTexCoords);
  float2 randValue = GetRandom(localTexCoords);
  float ao = 0.0f;
  float rad = mSample_radius / p.z;

  int iterators = 4;
  for (int j = 0; j < iterators; ++j) 
  {
    float2 coord1 = reflect(vec[j], randValue) * rad;
    
    float2 coord2 = float2(coord1.x * 0.707 - coord1.y * 0.707,
                           coord1.x * 0.707 + coord1.y * 0.707);

    ao += DoAmbientOcclusion(localTexCoords, coord1 * 0.25, p.xyz, n);
    ao += DoAmbientOcclusion(localTexCoords, coord2 * 0.5,  p.xyz, n);
    ao += DoAmbientOcclusion(localTexCoords, coord1 * 0.75, p.xyz, n);
    ao += DoAmbientOcclusion(localTexCoords, coord2, p.xyz, n);
  }

  ao /= (iterators * 4);
    
  bufferOut[index.xy] = float4(1 - ao.xxx, 1.0f);
}