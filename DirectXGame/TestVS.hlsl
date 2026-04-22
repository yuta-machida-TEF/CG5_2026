struct VertexShaderOutput
{
   float4 postion : SV_POSTION;
};

struct VertexShaderInput 
{
    float4 postion : POSTION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    vertexShaderOutput output;
    output.postion = input.postion;
    return output;
}
