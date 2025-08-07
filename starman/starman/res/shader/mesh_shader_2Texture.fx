// BOMありのUTF8だとコンパイルできなくなる。そのため、シェーダーファイルだけはBOMなし
float4x4 g_matWorld;
float4x4 g_matWorldViewProj;
float4 g_vecLightNormal;
float g_fLightBrigntness;
float4 g_vecDiffuse;
float4 g_vecAmbient = { 0.3f, 0.3f, 0.3f, 0.0f };
float4 g_vecCameraPos = { 0.0f, 0.0f, 0.0f, 0.0f };
texture g_texture;
texture g_mesh_texture2;

float g_fFogDensity;

float4 g_vecPointLightPos = { 1, 1, 1, 0};
bool g_bPointLightEnable;

bool g_inCaveFadeFinish = false;

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
    out_position  = mul(in_position, g_matWorldViewProj);

    float light_intensity = g_fLightBrigntness * dot(in_normal, g_vecLightNormal);

    float4 _ambient = g_vecAmbient;
    if (g_inCaveFadeFinish)
    {
        _ambient = 0.f;
    }

    out_diffuse = g_vecDiffuse * max(0, light_intensity) + _ambient;
    out_diffuse.r *= 0.7f; // 暗くしてみる
    out_diffuse.gb *= 0.5f; // 暗くしてみる
    out_diffuse.a = 1.0f;

    out_texcood = in_texcood;

    //----------------------------------
    // 霧の描画
    //----------------------------------
    // ワールド座標に変換
    float4 worldPos = mul(in_position, g_matWorld);

    // カメラからの距離をワールド空間で計算
    float distance = length(worldPos.xyz - g_vecCameraPos.xyz);

    float work = 1.0f - ((10000 - distance) / 10000);

    work *= g_fFogDensity;

    if (work >= 0.6f)
    {
        work = 0.6f;
    }

    fog_strength = work;

    out_worldPos = mul(in_position, g_matWorld).xyz;
    out_normal = mul(in_normal, g_matWorld).xyz;
}

float4 fog_color = { 0.5f, 0.3f, 0.2f, 1.0f };
float4 light_color = { 0.5f, 0.25f, 0.0f, 1.0f };

sampler mesh_texture_sampler = sampler_state {
    Texture   = (g_texture);
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MaxAnisotropy = 8;
};

sampler mesh_texture_sampler2 = sampler_state {
    Texture   = (g_mesh_texture2);
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MaxAnisotropy = 8;
};

// 多分、赤色成分が少ないテクスチャ画像を使うと、赤色部分の演算結果がオーバーフローして真っ白になる。
void pixel_shader(
    in float4 in_diffuse  : COLOR0,
    in float2 in_texcood  : TEXCOORD0,
    in float   fog : TEXCOORD1,
    in float3 in_worldPos : TEXCOORD2,
    in float3 in_normal : TEXCOORD3,
    out float4 out_diffuse : COLOR0
    )
{
    float4 color_result = (float4)0;
    float4 color_result2 = (float4)0;

    float2 in_texcood2 = in_texcood;
    in_texcood2 *= 300.f;

    color_result = tex2D(mesh_texture_sampler, in_texcood);
    color_result2 = tex2D(mesh_texture_sampler2, in_texcood2);

//    out_diffuse = (in_diffuse * color_result);
    color_result = lerp(color_result2, color_result, color_result.a);
    out_diffuse = (in_diffuse * color_result);

//    if (out_diffuse.r == 0.0f && out_diffuse.r == 0.0f && out_diffuse.r == 0.0f)
//    {
//        out_diffuse = in_diffuse;
//    }

    //------------------------------------------------------
    // 霧の描画
    //
    // 霧はピクセルシェーダーでやらないと意味がない。
    // 頂点シェーダーでやると、遠いほど輝いて見えるようになってしまう
    //------------------------------------------------------
    float4 fog_color2 = fog_color * g_fLightBrigntness;

    out_diffuse = (out_diffuse * (1.f - fog)) + (fog_color2 * fog);

    // 夜空は青色にしたい
    out_diffuse.rg *= (g_fLightBrigntness * 1.414f);
    out_diffuse.b *= (2.f - g_fLightBrigntness);

    //------------------------------------------------------
    // ポイントライト
    //------------------------------------------------------
    if (g_bPointLightEnable)
    {
        // 距離減衰の計算
        float distance = length((float3)g_vecPointLightPos - in_worldPos);

        // 適当に2乗減衰
        float attenuation = 50.0 / (distance * distance);
        attenuation = min(attenuation, 1.0);

        // 最終カラー
        out_diffuse += color_result * light_color * attenuation;
    }

}

technique Technique1
{
    pass Pass1
    {

        //AlphaBlendEnable = TRUE;
        //SrcBlend = SRCALPHA;
        //DestBlend = INVSRCALPHA;

        VertexShader = compile vs_3_0 vertex_shader();
        PixelShader  = compile ps_3_0 pixel_shader();
    }
}
