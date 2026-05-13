struct VertexShaderOutput
{
   float4 postion : SV_POSITION;
};

struct VertexShaderInput 
{
    float4 postion : POSTION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.postion = input.postion;
    return output;
}
