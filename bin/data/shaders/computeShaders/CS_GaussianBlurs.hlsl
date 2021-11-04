/*
* Shader for DirectX 11.
* Compute shader BLURS
* Made by: Ramses Guerrero Ambriz.
*/

//Input data
//----------------------------------------------------------------------------
//Rougnness
Texture2D<float4> InputImage : register(t0);

SamplerState simpleSampler : register(s0);


//Global info
//----------------------------------------------------------------------------
float g_gamma = 1.0f;

//Constant buffers
//----------------------------------------------------------------------------
cbuffer linkToBufferViewportDimension : register(b0)
{
  float2 viewportDimensions;
  float2 mNothing;
}

//Compute
//----------------------------------------------------------------------------
RWTexture2D<float4> TextureBlur : register(u0);

//Functions
//----------------------------------------------------------------------------
[numthreads(32, 32, 1)]
void
cs_gaussian_blurH(uint3 groupID          : SV_GroupID, 
                  uint3 groupThreadID    : SV_GroupThreadID,
			      uint groupIndex        : SV_GroupIndex,
			      uint3 dispatchThreadID : SV_DispatchThreadID) {
				  
  if(dispatchThreadID.x > viewportDimensions.x || dispatchThreadID.y > viewportDimensions.y) {
    return;
  }

  uint2 indexColor = (dispatchThreadID.xy);
  float2 index = (dispatchThreadID.xy / viewportDimensions);

  float offset[3] = {0.0, 1.3846153846, 3.2307692308};
  float weight[3] = {0.2270270270, 0.316216162, 0.0702702703};
  
  float4 FragmentColor = pow(InputImage.SampleLevel(simpleSampler, index, 0), g_gamma) * weight[0];
  
  for(int i = 1; i < 3; i++) {
    FragmentColor += pow(InputImage.SampleLevel(simpleSampler, index + (float2(offset[i], 0.0f) / 
	                     viewportDimensions), 0), g_gamma) * weight[i];
	FragmentColor += pow(InputImage.SampleLevel(simpleSampler, index - (float2(offset[i], 0.0f) / 
	                     viewportDimensions), 0), g_gamma) * weight[i];
  }
  
  TextureBlur[indexColor] = pow(FragmentColor, 1.0f/g_gamma);
}

//----------------------------------------------------------------------------
[numthreads(32, 32, 1)]
void
cs_gaussian_blurV(uint3 groupID          : SV_GroupID, 
                  uint3 groupThreadID    : SV_GroupThreadID,
			      uint groupIndex        : SV_GroupIndex,
			      uint3 dispatchThreadID : SV_DispatchThreadID) {

  if(dispatchThreadID.x > viewportDimensions.x || dispatchThreadID.y > viewportDimensions.y) {
    return;
  }

  uint2 indexColor = (dispatchThreadID.xy);
  float2 index = (dispatchThreadID.xy / viewportDimensions);
  
  float offset[3] = {0.0, 1.3846153846, 3.2307692308};
  float weight[3] = {0.2270270270, 0.316216162, 0.0702702703};
  
  float4 FragmentColor = pow(InputImage.SampleLevel(simpleSampler, index, 0), g_gamma) * weight[0];
  
  for(int i = 1; i < 3; i++) {
    FragmentColor += pow(InputImage.SampleLevel(simpleSampler, index + (float2(0.0f, offset[i]) / 
	                     float2((viewportDimensions).x,(viewportDimensions).y)), 0), g_gamma) * weight[i];
	FragmentColor += pow(InputImage.SampleLevel(simpleSampler, index - (float2(0.0f, offset[i]) / 
	                     float2((viewportDimensions).x,(viewportDimensions).y)), 0), g_gamma) * weight[i];
  }
  
  TextureBlur[indexColor] = pow(FragmentColor, 1.0f/g_gamma);
}