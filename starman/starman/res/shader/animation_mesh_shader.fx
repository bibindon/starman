// BOMありのUTF8だとコンパイルできなくなる。そのため、シェーダーファイルだけはBOMなし
float4x4 g_world;
float4x4 g_world_view_projection;
float4 g_light_normal;
float g_light_brightness;
float4 g_diffuse;
float4 g_ambient = { 0.5f, 0.5f, 0.5f, 0.0f };
texture g_mesh_texture;

void vertex_shader(
    in  float4 in_position  : POSITION,
    in  float4 in_normal    : NORMAL0,
    in  float4 in_texcood   : TEXCOORD0,

    out float4 out_position : POSITION,
    out float4 out_diffuse  : COLOR0,
    out float4 out_texture  : TEXCOORD0)
{
    out_position  = mul(in_position, g_world_view_projection);
    in_normal = normalize(in_normal);

    // ハーフランバート
    float dot_ = dot(in_normal, g_light_normal);
    dot_ += 1.f;
    dot_ *= 0.5f;

    float light_intensity = g_light_brightness * dot_;
    out_diffuse = g_diffuse * max(0, light_intensity) + g_ambient;
    out_diffuse.r *= 0.7f; // 暗くしてみる
    out_diffuse.gb *= 0.5f; // 暗くしてみる
    out_diffuse.a = 1.0f;

    out_texture  = in_texcood;
}

sampler mesh_texture_sampler = sampler_state
{
    Texture   = (g_mesh_texture);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

void pixel_shader(
    in  float4 in_diffuse     : COLOR0,
    in  float2 in_texcood     : TEXCOORD0,
    out float4 out_diffuse    : COLOR0)
{
    float4 color_result = (float4)0;
    color_result = tex2D(mesh_texture_sampler, in_texcood);
    out_diffuse = (in_diffuse * color_result);
}

technique technique_
{
    pass pass_
    {
        VertexShader = compile vs_3_0 vertex_shader();
        PixelShader  = compile ps_3_0 pixel_shader();
    }
}

