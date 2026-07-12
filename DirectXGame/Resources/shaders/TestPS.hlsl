#include "Test.hlsli"

Texture2D<float32_t4> gTexture : register(t0); //SRV
SamplerState gSampler : register(s0);//Sampler

struct PixelShaderOutput
{
     float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t2 uv = input.texcoord;
    float32_t4 textureColor = gTexture.Sample(gSampler,uv);

   //R(Red)赤 G(Green)緑 B(Blue)青 A(Alpha)透明度
   //位置セット(x y z w) か　カラーセット( r g b a )でアクセスできる
   output.color = textureColor;
   output.color.g = 0.0f;
   output.color.b = 0.0f;
   output.color.a = 1.0f;

   return output;
}
