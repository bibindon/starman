// BOMありのUTF8だとコンパイルできなくなる。そのため、シェーダーファイルだけはBOMなし
// 陰を表示しないシェーダー
float4x4 g_world_view_projection;
float4 g_diffuse = { 0.1f, 0.1f,0.1f, 0.5f };

void vertex_shader(
    in  float4 in_position  : POSITION,
    out float4 out_position : POSITION)
{
    out_position  = mul(in_position, g_world_view_projection);
}

void pixel_shader(
    in  float4 in_diffuse  : COLOR0,
    out float4 out_diffuse : COLOR0
    )
{
    out_diffuse = g_diffuse;
}

technique technique_
{
    pass pass_
    {
        VertexShader = compile vs_3_0 vertex_shader();
        PixelShader  = compile ps_3_0 pixel_shader();
    }
}
