/*
* Shader for DirectX 11.
* Depth shader
* Made by: Ramses Guerrero Ambriz.
*/

//----------------------------------------------------------------------------VS
cbuffer linkToBufferMatrixB : register(b0)
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};


//----------------------------------------------------------------------------VS
struct VS_INPUT
{
  float4 position    : POSITION0;
  float3 normal      : NORMAL0;
  float3 tangent     : TANGENT0;
  float3 biTangent   : BINORMAL0;
  float2 texCoords   : TEXCOORD0;
  float4 boneWeights : BLENDWEIGHT0;
  uint4  boneIds     : BLENDINDICES0;
};

struct PS_INPUT
{
  float4 position      : SV_POSITION;
  float depthPosition : TEXCOORD0;
};


//----------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------
PS_INPUT DepthVS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;

  matrix newMatWorld;
  newMatWorld[0] = float4(1, 0, 0, 0);
  newMatWorld[1] = float4(0, 1, 0, 0);
  newMatWorld[2] = float4(0, 0, 1, 0);
  newMatWorld[3] = float4(0, 0, 0, 1);

  matrix matWV = mul(newMatWorld, viewMatrix);
  
  output.depthPosition = mul(float4(input.position.xyz, 1.0f), matWV).z;
  
  matrix matClip = mul(matWV, projectionMatrix);

  //output.position = mul(input.position, matWV);
  output.position = mul(float4(input.position.xyz, 1.0f), matClip);
  
  // Change the position vector to be 4 units for proper matrix calculations.
  //input.position.w = 1.0f;
  
  // Calculate the position of the vertex against the world, view, and projection matrices.
  //output.position = mul(input.position, worldMatrix);
  //output.position = mul(output.position, viewMatrix);
  
  // Store the position value in a second input value for depth value calculations.
  //output.depthPosition = output.position.z;
  
  //output.position = mul(output.position, projectionMatrix);

  return output;
}


//----------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------
float4 DepthPS(PS_INPUT input) : SV_TARGET
{
  return float4(input.depthPosition, input.depthPosition, input.depthPosition, 1.0f);
}