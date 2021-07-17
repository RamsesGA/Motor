cbuffer ConstantBuffer1 : register( b0 )
{
    matrix View;
    matrix Projection;
};

cbuffer ConstantBuffer2 : register( b1 )
{
    matrix World;
    float4 vMeshColor;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoords : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.position  = mul( float4(input.position, 1.0f), World );
    output.position  = mul( output.position , View );
    output.position  = mul( output.position , Projection );
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    return vMeshColor;
}
