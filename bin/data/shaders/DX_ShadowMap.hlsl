/*
* Shader for DirectX 11.
* Shadow Map
* Made by: Ramses Guerrero Ambriz.
*/

//----------------------------------------------------------------------------PS
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState SampleTypeClamp : register(s1);
SamplerState SampleTypeWrap : register(s2);


//----------------------------------------------------------------------------VS
cbuffer linkToBufferShadows : register(b0)
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
  matrix lightViewMatrix;
  matrix lightProjectionMatrix;
}

cbuffer linkToBufferLight : register(b1)
{
  float3 lightPosition;
  float padding;
}

//----------------------------------------------------------------------------PS
cbuffer linkToBufferLight2 : register(b2)
{
  float4 ambientColor;
  float4 diffuseColor;
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


//----------------------------------------------------------------------------PS
struct PS_INPUT
{
  float4 position          : SV_POSITION;
  float3 normal            : NORMAL0;
  float2 tex               : TEXCOORD0;
  float4 lightViewPosition : TEXCOORD1;
  float3 lightPos          : TEXCOORD2;
};

struct PS_OUTPUT
{
  float4 mColor : SV_Target0;
};


//----------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------
PS_INPUT ShadowVS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;
  float4 worldPosition;
  
  // Change the position vector to be 4 units for proper matrix calculations.
  input.position.w = 1.0f;
  
  // Calculate the position of the vertex against the world, view, and projection matrices.
  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
  
  //
  //Here we transform the vertex based on the light's perspective
  //
  
  // Calculate the position of the vertice as viewed by the light source.
  output.lightViewPosition = mul(input.position, worldMatrix);
  output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
  output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);
  
  // Store the texture coordinates for the pixel shader.
  output.tex = input.texCoords;
  
  // Calculate the normal vector against the world matrix only.
  output.normal = mul(input.normal, (float3x3)worldMatrix);
  
  // Normalize the normal vector.
  output.normal = normalize(output.normal);
  
  // Calculate the position of the vertex in the world.
  worldPosition = mul(input.position, worldMatrix);
  
  // Determine the light position based on the position of the light and the position of the vertex in the world.
  output.lightPos = lightPosition.xyz - worldPosition.xyz;
  
  // Normalize the light position vector.
  output.lightPos = normalize(output.lightPos);
  
  return output;
}


//----------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------
PS_OUTPUT ShadowPS(PS_INPUT input) : SV_TARGET
{
  float bias;
  float depthValue;
  float lightDepthValue;
  float lightIntensity;
  float2 projectTexCoord;
  float4 textureColor;
  float4 color;
  
  PS_OUTPUT output;

  // Set the bias value for fixing the floating point precision issues.
  bias = 0.001f;

  // Set the default output color to the ambient light value for all pixels.
  color = ambientColor;

  // Calculate the projected texture coordinates.
  projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
  projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

  // Determine if the projected coordinates are in the 0 to 1 range.  
  // If so then this pixel is in the view of the light.
  if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
  {
    // Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
    depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;
    	
    // Calculate the depth of the light.
    lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
    
    // Subtract the bias from the lightDepthValue.
    lightDepthValue = lightDepthValue - bias;
    
    // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    // If the light is in front of the object then light the pixel,
    // if not then shadow this pixel since an object (occluder) is casting a shadow on it.
    if(lightDepthValue < depthValue)
    {
      // Calculate the amount of light on this pixel.
      lightIntensity = saturate(dot(input.normal, input.lightPos));
      
      if(lightIntensity > 0.0f)
      {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
	    
        // Saturate the final light color.
        color = saturate(color);
      }
    }
  }
  
  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);
  
  // Combine the light and texture color.
  color = color * textureColor;
  
  output.mColor = color;
  
  return output;
}