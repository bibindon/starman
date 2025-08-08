// BOMありのUTF8だとコンパイルできなくなる。そのため、シェーダーファイルだけはBOMなし

#include "Common.fx"

float4x4 g_matWorld;
float4x4 g_matWorldViewProj;

float4 g_vecLightNormal;
float g_fLightBrigntness;

float4 g_vecDiffuse;

float4 g_vecCameraPos = { 0.0f, 0.0f, 0.0f, 0.0f };

texture g_texture;
texture g_texture2;

float g_fFogDensity;

float4 g_vecPointLightPos = { 1, 1, 1, 0};
bool g_bPointLightEnable;

bool g_bCaveFadeFinish = false;

float4 g_vecFogColor = { 0.5f, 0.3f, 0.2f, 1.0f };
float4 g_vecLightColor = { 0.5f, 0.25f, 0.0f, 1.0f };

sampler g_samplerMeshTexture = sampler_state
{
    Texture   = (g_texture);
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MaxAnisotropy = 8;
};

sampler mesh_texture_sampler2 = sampler_state
{
    Texture   = (g_texture2);
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MaxAnisotropy = 8;
};

void VertexShader1(in  float4 inPos         : POSITION,
                   in  float4 inNormal      : NORMAL0,
                   in  float4 inTexCoord    : TEXCOORD0,

                   out float4 outPos        : POSITION,
                   out float4 outDiffuse    : COLOR0,
                   out float4 outTexCoord   : TEXCOORD0,
                   out float outFogStrength : TEXCOORD1,
                   out float3 outWorldPos   : TEXCOORD2,
                   out float3 outNormal     : TEXCOORD3)
{
    outPos  = mul(inPos, g_matWorldViewProj);

    float fLightIntensity = g_fLightBrigntness * dot(inNormal, g_vecLightNormal);

    float4 _ambient = g_vecAmbient;
    if (g_bCaveFadeFinish)
    {
        _ambient = 0.f;
    }

    outDiffuse = g_vecDiffuse * max(0, fLightIntensity) + _ambient;

    outTexCoord = inTexCoord;

    //----------------------------------
    // 霧の描画
    //----------------------------------
    // ワールド座標に変換
    float4 worldPos = mul(inPos, g_matWorld);

    // カメラからの距離をワールド空間で計算
    float distance = length(worldPos.xyz - g_vecCameraPos.xyz);

    float work = 1.0f - ((1000 - distance) / 1000);

    work *= g_fFogDensity;

    if (work >= 0.7f)
    {
        work = 0.7f;
    }

    outFogStrength = work;

    outWorldPos = mul(inPos, g_matWorld).xyz;
    outNormal = mul(inNormal, g_matWorld).xyz;
}

// 多分、赤色成分が少ないテクスチャ画像を使うと、赤色部分の演算結果がオーバーフローして真っ白になる。
void PixelShader1(in float4 inDiffuse    : COLOR0,
                  in float2 inTexCoord   : TEXCOORD0,
                  in float  inFogDensity : TEXCOORD1,
                  in float3 inWorldPos   : TEXCOORD2,
                  in float3 inNormal     : TEXCOORD3,
                  out float4 outDiffuse  : COLOR0)
{
    float4 vecResultColor = float4(0.f, 0.f, 0.f, 0.f);
    float4 vecResultColor2 = float4(0.f, 0.f, 0.f, 0.f);

    float2 inTexCoord2 = inTexCoord;
    inTexCoord2 *= 300.f;

    vecResultColor = tex2D(g_samplerMeshTexture, inTexCoord);
    vecResultColor2 = tex2D(mesh_texture_sampler2, inTexCoord2);

    vecResultColor = lerp(vecResultColor2, vecResultColor, vecResultColor.a);
    outDiffuse = (inDiffuse * vecResultColor);

    //------------------------------------------------------
    // 霧の描画
    //
    // 霧はピクセルシェーダーでやらないと意味がない。
    // 頂点シェーダーでやると、遠いほど輝いて見えるようになってしまう
    //------------------------------------------------------
    float4 fog_color2 = g_vecFogColor * g_fLightBrigntness;

    outDiffuse = (outDiffuse * (1.f - inFogDensity)) + (fog_color2 * inFogDensity);

    // 夜空は青色にしたい
    outDiffuse.rg *= (g_fLightBrigntness * 1.414f);
    outDiffuse.b *= (2.f - g_fLightBrigntness);

    //------------------------------------------------------
    // ポイントライト
    //------------------------------------------------------
    if (g_bPointLightEnable)
    {
        // ポイントライトまでの距離
        float distance = length((float3)g_vecPointLightPos - inWorldPos);

        // 遠いほど小さくなるようにする
        // attenuation...減衰
        float attenuation = 50.0f / (distance * distance);

        if (attenuation > 2.0f)
        {
            attenuation = 2.0f;
        }

        outDiffuse += vecResultColor * g_vecLightColor * attenuation;
    }
}

technique Technique1
{
    pass Pass1
    {
        VertexShader = compile vs_3_0 VertexShader1();
        PixelShader  = compile ps_3_0 PixelShader1();
    }
}
