/*
* Shader for DirectX 11.
* Compute shader CUDA
* Made by: Ramses Guerrero Ambriz.
*/
#define TOTAL_PIXELS 256

//Input data
//----------------------------------------------------------------------------
Texture2D<float4> inTexture : register(t0);

//Structures
//----------------------------------------------------------------------------
struct RG_Buffer {
  uint amountR[TOTAL_PIXELS];
  uint amountG[TOTAL_PIXELS];
};

struct BA_Buffer {
  uint amountB[TOTAL_PIXELS];
  uint amountA[TOTAL_PIXELS];
};

//Constant buffers
//----------------------------------------------------------------------------
cbuffer linkToBufferHistogram : register(b0)
{
  float4 mViewportDimensions;
}

//Compute buffers
//----------------------------------------------------------------------------
RWStructuredBuffer<RG_Buffer> RG_OutBuffer : register(u0);

RWStructuredBuffer<BA_Buffer> BA_OutBuffer : register(u1);

//MAIN
//----------------------------------------------------------------------------
[numthreads(32, 32, 1)]
void 
cs_CUDA(uint3 groupID          : SV_GroupID, 
        uint3 groupThreadID    : SV_GroupThreadID,
		uint groupIndex        : SV_GroupIndex,
		uint3 dispatchThreadID : SV_DispatchThreadID){
			 
  if ((dispatchThreadID.x > mViewportDimensions.x)||
       (dispatchThreadID.y > mViewportDimensions.y)){
    return;
  }

  uint index = dispatchThreadID.x;
  uint2 indexRGBA = dispatchThreadID.xy;
  
  InterlockedAdd(RG_OutBuffer[index].amountR[inTexture[indexRGBA].r], 1);
  InterlockedAdd(RG_OutBuffer[index].amountG[inTexture[indexRGBA].g], 1);
  
  InterlockedAdd(BA_OutBuffer[index].amountB[inTexture[indexRGBA].b], 1);
  InterlockedAdd(BA_OutBuffer[index].amountA[inTexture[indexRGBA].a], 1);
}