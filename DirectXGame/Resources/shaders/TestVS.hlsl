struct VertexShaderOutput
{
   float32_t4 postion : SV_POSITION;
};

struct VertexShaderInput 
{
    float32_t4 postion : POSTION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.postion = input.postion;
    return output;
}
