//--------------------------------------------
// 陰を表示しないシェーダー
//
// BOMありのUTF8だとコンパイルできなくなる。そのため、シェーダーファイルだけはBOMなし
//--------------------------------------------

float4x4 g_matWorldViewProj;
float4 g_vecDiffuse = { 0.0f, 0.0f,0.0f, 0.0f };

void vertex_shader(in  float4 inPos  : POSITION,
                   out float4 outPos : POSITION)
{
    outPos  = mul(inPos, g_matWorldViewProj);
}

void pixel_shader(in  float4 inDiffuse  : COLOR0,
                  out float4 outDiffuse : COLOR0)
{
    outDiffuse = g_vecDiffuse;
}

technique Technique1
{
    pass Pass1
    {
        VertexShader = compile vs_3_0 vertex_shader();
        PixelShader  = compile ps_3_0 pixel_shader();
    }
}
