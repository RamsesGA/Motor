/*
* Shader for DirectX 11.
* Blur (gaussyan)
* Made by: Ramses Guerrero Ambriz.
*/

//----------------------------------------------------------------------------
Texture2D inputImage : register(t0);

SamplerState simpleSampler : register(s0);
//----------------------------------------------------------------------------
cbuffer linkToBufferViewportDimension: register(b2)
{
  float2 viewportDimensions;
  float2 mNothing;
}

//----------------------------------------------------------------------------
struct PS_INPUT
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;
};

//----------------------------------------------------------------------------
float g_gamma = 1.0f;

float4 ps_gaussian_blurH(PS_INPUT input) : SV_Target0
{
  float offset[3] = {0.0, 1.3846153846, 3.2307692308};
  float weight[3] = {0.2270270270, 0.316216162, 0.0702702703};
  
  float4 fragmentColor = pow(inputImage.Sample(simpleSampler, input.texCoord), g_gamma)
                             * weight[0];
  
  for(int i = 1; i < 3; i++)
  {
    fragmentColor += pow(inputImage.Sample(simpleSampler, input.texCoord + 
                        (float2(offset[i], 0.0f) / viewportDimensions)), g_gamma) * weight[i];
    
    fragmentColor += pow(inputImage.Sample(simpleSampler, input.texCoord - 
                        (float2(offset[i], 0.0f) / viewportDimensions)), g_gamma) * weight[i];
  }
  
  fragmentColor = pow(fragmentColor, 1.0f/g_gamma);
  return fragmentColor;
}

float4 ps_gaussian_blurV(PS_INPUT input) : SV_Target0
{
  float offset[3] = {0.0, 1.3846153846, 3.2307692308};
  float weight[3] = {0.2270270270, 0.316216162, 0.0702702703};
  
  float4 fragmentColor = pow(inputImage.Sample(simpleSampler, input.texCoord), g_gamma) 
                             * weight[0];
  
  for(int i = 1; i < 3; i++)
  {
    fragmentColor += pow(inputImage.Sample(simpleSampler, input.texCoord + 
                        (float2(0.0f, offset[i]) / viewportDimensions)), g_gamma) * weight[i];

	  fragmentColor += pow(inputImage.Sample(simpleSampler, input.texCoord - 
                        (float2(0.0f, offset[i]) / viewportDimensions)), g_gamma) * weight[i];
  }
  
  fragmentColor = pow(fragmentColor, 1.0f/g_gamma);
  return fragmentColor;
}