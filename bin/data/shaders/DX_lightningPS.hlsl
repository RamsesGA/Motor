/*
* Shader for DirectX 11.
* Lighting Shader
* Made by: Ramses Guerrero Ambriz.
*/

#define M_PI 3.14159265383
#define EPSILON 0.00001
#define SHADOW_BIAS 0.001

Texture2D baseColor : register(t0);
Texture2D normalTex : register(t1);
Texture2D emissiveTex : register(t2);
Texture2D AOTexture : register(t3);
Texture2D posTexture : register(t4);
Texture2D shadowTexture : register(t5);

//TextureCube IBL_specular;
//TextureCube IBL_diffuse;

SamplerState simpleSampler : register(s0);
SamplerState sampleTypeClamp : register(s1);

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

cbuffer linkToBufferLightning : register(b2)
{
  float emissiveIntensity;
  float lightIntensity;
  
  float lightPosX;
  float lightPosY;
  float lightPosZ;
  
  float vViewPositionX;
  float vViewPositionY;
  float vViewPositionZ;
}

cbuffer linkToBufferInverse : register(b3)
{
  matrix mInverseView;
  matrix mInverseProjection;
}

cbuffer linkToBufferShadows : register(b4)
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
  matrix lightViewMatrix;
  matrix lightProjectionMatrix;
}

//----------------------------------------------------------------------------
struct PS_INPUT
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;
};

//----------------------------------------------------------------------------
float3 fresnelSchlick(float3 F0, float cosTheta)
{
  return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

//----------------------------------------------------------------------------
float ndf_GGX(float NdH, float roughness)
{
  float alpha = roughness * roughness;
  float alphaSqr = alpha * alpha;
  
  float denom = (NdH * NdH) * (alphaSqr - 1.0f) + 1.0f;
  
  return alphaSqr / (M_PI * denom * denom);
}

//----------------------------------------------------------------------------
float ga_SchlickG1(float cosTheta, float k)
{
  return cosTheta / (cosTheta * (1.0f - k) + k);
}

//----------------------------------------------------------------------------
float ga_SchlickGGX(float cosLi, float cosLo, float roughness)
{
  float r = roughness + 1.0f;
  //Epic suggest using this roughness remaping for analytic lights
  float k = (r * r) / 8.0f;
  
  return ga_SchlickG1(cosLi, k) * ga_SchlickG1(cosLo, k);
}

//----------------------------------------------------------------------------
float4 ps_main(PS_INPUT input) : SV_Target0
{
  float gamma = 2.2f;
  
  float4 posWorld = float4(posTexture.Sample(simpleSampler, input.texCoord).xyz, 1.0f);
  float4 posWorldView = mul(posWorld, mView);  

  float4x4 matWV = mul(mWorld, mView);
  
  float4 normal = normalTex.Sample(simpleSampler, input.texCoord);
  float roughness = normal.w;
  normal.w = 0.0f;
  
  float4 diffuse = baseColor.Sample(simpleSampler, input.texCoord);
  diffuse.xyz = pow(diffuse.xyz, gamma);
  
  float metallic = diffuse.w;
  diffuse.w = 1.0f;
  
  //Emmisive & AO
  float4 emissive = pow(emissiveTex.Sample(simpleSampler, input.texCoord), gamma);
  float ao = AOTexture.Sample(simpleSampler, input.texCoord).r;
  
  float3 specular_F0 = lerp(0.04f, diffuse.xyz, metallic);
  
  //Light
  float4 wvLightPos = mul(float4(lightPosX, lightPosY, lightPosZ, 1.0f), matWV);
  float4 wvViewPosition = mul(float4(vViewPositionX, vViewPositionY, vViewPositionZ, 1.0f), matWV);
  
  float3 LightDir = -normalize(wvLightPos.xyz - posWorld.xyz);
  float3 ViewDir = normalize(wvViewPosition.xyz - posWorld.xyz);
  
  float NdL = max(0.0f, dot(normal.xyz, LightDir));
  float NdV = max(0.0f, dot(normal.xyz, ViewDir));
  
  float3 Half = normalize(ViewDir + LightDir);
  
  float NdH = max(0.001f, dot(normal.xyz, Half));
  float HdL = saturate(dot(Half, LightDir));
  float HdV = saturate(dot(Half, ViewDir));
  
  float3 Reflect = normalize(reflect(-ViewDir, normal));
  
  float D = ndf_GGX(NdH, roughness);
  float3 F = fresnelSchlick(specular_F0, HdL);
  float G = ga_SchlickGGX(NdL, NdV, roughness);
  
  //Specular
  float3 specular = (D * F * G) / max(EPSILON,(NdL * NdV * 4));
  
  //Ambient light
  //float3 ambientLighting;
  //{
  //  float3 F_a = fresnelSchlick(specular, NdV);
  //  float3 kd_a = lerp(1.0f - F_a, 0.0f, metallic);
  //
  //  float mipLevel = (1.0f - roughness * roughness) * 8.0;
  //
  //  float4 vecReflectionSelected = float4(Reflect, mipLevel);
  //  float4 siColor = pow((IBL_specular.Sample(simpleSampler, vecReflectionSelected)), gamma);
  //  float4 diColor = pow((IBL_diffuse.Sample(simpleSampler, normal.xyz)), gamma);
  //
  //  float3 diffuseAmbient = diColor.xyz * kd_a * diffuse;
  //
  //  ambientLighting = (siColor * specular_F0) + diffuseAmbient;
  //}
  
  //Shadow
  float localShadow = 1.0f;
  posWorld.w = 1.0f;

  float4 vertexPos = mul(posWorld, mView);
  vertexPos = mul(vertexPos, mProjection);

  float4 shadowPos = mul(posWorld, mInverseView);
  shadowPos = mul(shadowPos, lightViewMatrix);

  float4 shadowWPos = shadowPos;
  float4 shadowClipPos = mul(shadowWPos, lightProjectionMatrix);
  
  shadowClipPos /= shadowClipPos.w;

  float2 shadowTexCoords = 0.5f + (shadowClipPos.xyz * 0.5f);
  shadowTexCoords.x = shadowClipPos.x / 2.0f + 0.5f;
  shadowTexCoords.y = -shadowClipPos.y / 2.0f + 0.5f;

  float finalShadow = shadowTexture.Sample(sampleTypeClamp, shadowTexCoords.xy).x;

  float currentDepth = shadowClipPos.z - SHADOW_BIAS;
  //currentDepth = vertexPos.z / vertexPos.w;

  localShadow = currentDepth < finalShadow ? 0.0f : 1.0f;

  if (currentDepth < finalShadow) { 
    //localShadow = 0.0f;
  }

  float4 finalColor = float4(pow((1.0f - localShadow) * (diffuse.xyz * NdL * lightIntensity) + 
                                 (emissive.xyz * emissiveIntensity) + (specular), 1.0f / gamma), 1.0f);
								 
  finalColor = finalColor * ao;
  return finalColor;
}