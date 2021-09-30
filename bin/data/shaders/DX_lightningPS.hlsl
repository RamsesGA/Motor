/*
* Shader for DirectX 11.
* Lighting Shader
* Made by: Ramses Guerrero Ambriz.
*/

#define M_PI 3.14159265383
#define EPSILON 0.00001

Texture2D baseColor     : register(t0);
Texture2D normalTex     : register(t1);
Texture2D emissiveTex   : register(t2);
Texture2D AOTexture     : register(t3);
Texture2D posTexture    : register(t4);
Texture2D shadowTexture : register(t5);

TextureCube IBL_specular;
TextureCube IBL_diffuse;

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
  float4 objectPosition;
}

cbuffer linkToCBLightning : register(b2)
{
  float emissiveIntensity;
  float lightIntensity0;
  float3 lightPos0;
  float3 vViewPosition;
}

cbuffer linkToCBInverseCam : register(b3)
{
  matrix mInverseView;
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
  
  float4x4 matWV = mul(mWorld, mView);
  
  float4 normal = normalTex.Sample(simpleSampler, input.texCoord);
  float roughness = normal.w;
  normal.w = 0.0f;
  
  float4 diffuse = baseColor.Sample(simpleSampler, input.texCoord);
  diffuse.xyz = pow(diffuse.xyz, gamma);
  
  float metallic = diffuse.w;
  diffuse.w = 1.0f;
  
  float4 emissive = pow(emissiveTex.Sample(simpleSampler, input.texCoord), gamma);
  float ao = AOTexture.Sample(simpleSampler, input.texCoord).r;
  
  float3 specular_F0 = lerp(0.04f, diffuse.xyz, metallic);
  
  //float4(lightPos0, -255.0f) float4(-100.0f, 100.0f, -100.0f, 1.0f)
  float3 wvLightPos_0 = mul(float4(-100.0f, 100.0f, -100.0f, 1.0f), matWV);
  float4 wvViewPosition = mul(vViewPosition, matWV);
  
  float3 LightDir = normalize(wvLightPos_0.xyz - posWorld.xyz);
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
  
  float3 specular = (D * F * G) / max(EPSILON,(NdL * NdV * 4));
  
  float3 ambientLighting;
  {
    float3 F_a = fresnelSchlick(specular, NdV);
    float3 kd_a = lerp(1.0f - F_a, 0.0f, metallic);
  
    float mipLevel = (1.0f - roughness * roughness) * 8.0;
  
    float4 vecReflectionSelected = float4(Reflect, mipLevel);
    float4 siColor = pow((IBL_specular.Sample(simpleSampler, vecReflectionSelected)), gamma);
    float4 diColor = pow((IBL_diffuse.Sample(simpleSampler, normal.xyz)), gamma);
  
    float3 diffuseAmbient = diColor.xyz * kd_a * diffuse;
  
    ambientLighting = (siColor * specular_F0) + diffuseAmbient;
  }
  
  //Shadow
  float localShadow = 1.0f;

  float4 inverseWorld = mul(posWorld, mInverseView);
  float4 shadowPos = mul(inverseWorld, lightViewMatrix);
  float4 shadowClipPos = mul(shadowPos, lightProjectionMatrix);
  shadowClipPos /= shadowClipPos.w;

  float2 shadowTexCoords = 0.5f + (shadowClipPos.xy * 0.5f);
  shadowTexCoords.y = 1 - shadowTexCoords.y;

  float finalShadow = shadowTexture.Sample(simpleSampler, shadowTexCoords).x;

  if ((shadowPos.z - 0.1f) > finalShadow) {
    localShadow = 0.0f;
  }
  
  return float4(localShadow.xxx, 1.0f);

  float4 finalColor = float4(pow((diffuse.xyz * NdL * lightIntensity0) + 
                                  (emissive.xyz * emissiveIntensity) + 
  							      (specular) + 
  							      (ambientLighting) + 
                                  (finalShadow),
  							      1.0f / gamma),
  						      1);
								 
  finalColor = finalColor * ao;
  return finalColor;
}