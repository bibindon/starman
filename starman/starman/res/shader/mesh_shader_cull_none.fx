float4x4 g_world;
float4x4 g_world_view_projection;
float4 g_light_normal;
float g_light_brightness;
float4 g_diffuse;
//float4 g_ambient = { 0.45f, 0.4f, 0.3f, 0.0f };
float4 g_ambient = { 0.1f, 0.1f, 0.1f, 0.0f };
float4 g_cameraPos = { 0.0f, 0.0f, 0.0f, 0.0f };
texture g_mesh_texture;

float4 g_point_light_pos = { 1, 1, 1, 0};
bool pointLightEnable;

void vertex_shader(
    in  float4 in_position  : POSITION,
    in  float4 in_normal    : NORMAL0,
    in  float4 in_texcood   : TEXCOORD0,

    out float4 out_position : POSITION,
    out float4 out_diffuse  : COLOR0,
    out float4 out_texcood  : TEXCOORD0,
    out float fog_strength : TEXCOORD1,
    out float3 out_worldPos : TEXCOORD2,
    out float3 out_normal : TEXCOORD3
    )
{
    out_position  = mul(in_position, g_world_view_projection);

    float light_intensity = g_light_brightness * dot(in_normal, g_light_normal);
    out_diffuse = g_diffuse * max(0, light_intensity) + g_ambient;
    out_diffuse.r *= 0.9f; // 暗くしてみる
    out_diffuse.gb *= 0.6f; // 暗くしてみる
    out_diffuse.a = 1.0f;

    out_texcood = in_texcood;

    //----------------------------------
    // 霧の描画
    //----------------------------------
    // ワールド座標に変換
    float4 worldPos = mul(in_position, g_world);

    // カメラからの距離をワールド空間で計算
    float distance = length(worldPos.xyz - g_cameraPos.xyz);

    float work = 1.0f - ((3000 - distance) / 3000);
    if (work >= 0.6f)
    {
        work = 0.6f;
    }

    fog_strength = work;

    out_worldPos = mul(in_position, g_world).xyz;
    out_normal = mul(in_normal, g_world).xyz;
}

float4 fog_color = { 0.5f, 0.3f, 0.2f, 1.0f };
float4 light_color = { 0.5f, 0.25f, 0.0f, 1.0f };

sampler mesh_texture_sampler = sampler_state {
    Texture   = (g_mesh_texture);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

void pixel_shader(
    in  float4 in_diffuse  : COLOR0,
    in  float2 in_texcood  : TEXCOORD0,
    in float   fog : TEXCOORD1,
    in float3 in_worldPos : TEXCOORD2,
    in float3 in_normal : TEXCOORD3,
    out float4 out_diffuse : COLOR0
    )
{
    float4 color_result = (float4)0;
    color_result = tex2D(mesh_texture_sampler, in_texcood);
    out_diffuse = (in_diffuse * color_result);
    if (out_diffuse.r == 0.0f && out_diffuse.r == 0.0f && out_diffuse.r == 0.0f)
    {
        out_diffuse = in_diffuse;
    }

    //------------------------------------------------------
    // 霧の描画
    //
    // 霧はピクセルシェーダーでやらないと意味がない。
    // 頂点シェーダーでやると、遠いほど輝いて見えるようになってしまう
    //------------------------------------------------------
    float4 fog_color2 = fog_color * g_light_brightness;


    out_diffuse = (out_diffuse * (1.f - fog)) + (fog_color2 * fog);

    // 夜空は青色にしたい
    out_diffuse.rg *= (g_light_brightness*1.414f);
    out_diffuse.b *= (2.f - g_light_brightness);

    //------------------------------------------------------
    // ポイントライト
    //
    // 霧の描画の逆をやればよい
    //------------------------------------------------------
    if (pointLightEnable)
    {
        /*
        if (fog <= 0.0016f)
        {
            float work = fog * 625.f;
            if (work >= 0.f)
            {
                // 自分の周りを黄色くする
                out_diffuse.r += (1 - (work*work))/2;
                out_diffuse.g += (1 - (work*work))/4.f;
            }
        }
        */

        // 法線を正規化
        float3 N = normalize(in_normal);

        // ライト方向を計算
        float3 L = normalize((float3)g_point_light_pos - in_worldPos);

        // 距離減衰の計算
        float distance = length((float3)g_point_light_pos - in_worldPos);

        // 適当に2乗減衰
        float attenuation = 50.0 / (distance * distance);
        attenuation = min(attenuation, 1.0);

        // ライトの強さ（ランバート反射）
        float NdotL = max(dot(N, L), 0);

        // 最終カラー
        out_diffuse += light_color * NdotL * attenuation;
    }
}

technique technique_
{
    pass pass_
    {
        CullMode = None;
        VertexShader = compile vs_2_0 vertex_shader();
        PixelShader  = compile ps_2_0 pixel_shader();
    }
}
