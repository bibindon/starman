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

bool g_bCaveFadeFinish = false;

void vertex_shader(
    in  float4 inPos  : POSITION,
    in  float4 inNormal    : NORMAL0,
    in  float4 inTexCoord   : TEXCOORD0,

    out float4 outPos : POSITION,
    out float4 outDiffuse  : COLOR0,
    out float4 outTexCoord  : TEXCOORD0,
    out float outFogStrength : TEXCOORD1,
    out float3 outWorldPos : TEXCOORD2,
    out float3 outNormal : TEXCOORD3

    )
{
    outPos  = mul(inPos, g_matWorldViewProj);

    float fLightIntensity = g_fLightBrigntness * dot(inNormal, g_vecLightNormal);

    float4 _ambient = g_vecAmbient;
    if (g_bCaveFadeFinish)
    {
        _ambient = 0.f;
    }

    outDiffuse = g_vecDiffuse * max(0, fLightIntensity) + _ambient;
    outDiffuse.r *= 0.7f; // 暗くしてみる
    outDiffuse.gb *= 0.5f; // 暗くしてみる
    outDiffuse.a = 1.0f;

    outTexCoord = inTexCoord;

    //----------------------------------
    // 霧の描画
    //----------------------------------
    // ワールド座標に変換
    float4 worldPos = mul(inPos, g_matWorld);

    // カメラからの距離をワールド空間で計算
    float distance = length(worldPos.xyz - g_vecCameraPos.xyz);

    float work = 1.0f - ((10000 - distance) / 10000);

    work *= g_fFogDensity;

    if (work >= 0.6f)
    {
        work = 0.6f;
    }

    outFogStrength = work;

    outWorldPos = mul(inPos, g_matWorld).xyz;
    outNormal = mul(inNormal, g_matWorld).xyz;
}

float4 g_vecFogColor = { 0.5f, 0.3f, 0.2f, 1.0f };
float4 g_vecLightColor = { 0.5f, 0.25f, 0.0f, 1.0f };

sampler g_samplerMeshTexture = sampler_state {
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
    in float4 inDiffuse  : COLOR0,
    in float2 inTexCoord  : TEXCOORD0,
    in float   fog : TEXCOORD1,
    in float3 inWorldPos : TEXCOORD2,
    in float3 inNormal : TEXCOORD3,
    out float4 outDiffuse : COLOR0
    )
{
    float4 color_result = (float4)0;
    float4 color_result2 = (float4)0;

    float2 in_texcood2 = inTexCoord;
    in_texcood2 *= 300.f;

    color_result = tex2D(g_samplerMeshTexture, inTexCoord);
    color_result2 = tex2D(mesh_texture_sampler2, in_texcood2);

//    outDiffuse = (inDiffuse * color_result);
    color_result = lerp(color_result2, color_result, color_result.a);
    outDiffuse = (inDiffuse * color_result);

//    if (outDiffuse.r == 0.0f && outDiffuse.r == 0.0f && outDiffuse.r == 0.0f)
//    {
//        outDiffuse = inDiffuse;
//    }

    //------------------------------------------------------
    // 霧の描画
    //
    // 霧はピクセルシェーダーでやらないと意味がない。
    // 頂点シェーダーでやると、遠いほど輝いて見えるようになってしまう
    //------------------------------------------------------
    float4 fog_color2 = g_vecFogColor * g_fLightBrigntness;

    outDiffuse = (outDiffuse * (1.f - fog)) + (fog_color2 * fog);

    // 夜空は青色にしたい
    outDiffuse.rg *= (g_fLightBrigntness * 1.414f);
    outDiffuse.b *= (2.f - g_fLightBrigntness);

    //------------------------------------------------------
    // ポイントライト
    //------------------------------------------------------
    if (g_bPointLightEnable)
    {
        // 距離減衰の計算
        float distance = length((float3)g_vecPointLightPos - inWorldPos);

        // 適当に2乗減衰
        float attenuation = 50.0 / (distance * distance);
        attenuation = min(attenuation, 1.0);

        // 最終カラー
        outDiffuse += color_result * g_vecLightColor * attenuation;
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
