/*
* Shader for DirectX 11.
* Shader histogram
* Made by: Ramses Guerrero Ambriz.
*/
#define TOTAL_PIXELS 256

//Constant buffers
//----------------------------------------------------------------------------
cbuffer linkToBufferHistogram : register(b0)
{
  float4 mViewportDimensions; //Viewport resolution (needed in pixels)
}

cbuffer linkToBufferRedGreen : register(b1)
{
  uint amountR[TOTAL_PIXELS];
  uint amountG[TOTAL_PIXELS];
}

cbuffer linkToBufferBlueAlpha: register(b2)
{
  uint amountB[TOTAL_PIXELS];
  uint amountA[TOTAL_PIXELS];
}

//Structs
//----------------------------------------------------------------------------
struct PS_INPUT
{
  float2 texCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
  float4 mColor : SV_Target0;
};

//MAIN
//----------------------------------------------------------------------------
PS_OUTPUT cs_histogram(PS_INPUT input) {
  PS_OUTPUT output;
  
  float2 uv = (input.texCoord / mViewportDimensions.xy);
  
  int pIndex = int(floor(uv.x * float(TOTAL_PIXELS)));

  float4 frequency;
  frequency.r = amountR[pIndex];
  frequency.g = amountG[pIndex];
  frequency.b = amountB[pIndex];
  frequency.a = amountA[pIndex];
  
  float val = uv.y * float(2000.0f);
  
  float rRes = step(val, frequency.r);
  float gRes = step(val, frequency.g);
  float bRes = step(val, frequency.b);
  float aRes = step(val, frequency.a);
    
  output.mColor = float4(rRes, gRes, bRes, aRes);
  return output;
}