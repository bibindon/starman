// BOMありのUTF8だとコンパイルできなくなる。そのため、シェーダーファイルだけはBOMなし

float4x4 g_matWorld;
float4x4 g_matWorldViewProj;

float4 g_vecLightNormal;
float4 g_vecLightColor = { 0.5f, 0.25f, 0.0f, 1.0f };
float g_fLightBrigntness;

float4 g_vecDiffuse;
float4 g_vecAmbient = { 0.3f, 0.3f, 0.3f, 0.0f };

float4 g_vecCameraPos = { 0.0f, 0.0f, 0.0f, 0.0f };

float g_fFogDensity;
float4 g_vecFogColor = { 0.5f, 0.3f, 0.2f, 1.0f };

float4 g_vecPointLightPos = { 1, 1, 1, 0};
bool g_bPointLightEnable;

bool g_bCaveFadeFinish = false;

texture g_texture;
sampler mesh_texture_sampler = sampler_state
{
    Texture   = (g_texture);
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MaxAnisotropy = 8;
};


void vertex_shader(in  float4 inPos          : POSITION,
                   in  float4 inNormal       : NORMAL0,
                   in  float4 inTexCoord     : TEXCOORD0,

                   out float4 outPos         : POSITION,
                   out float4 outDiffuse     : COLOR0,
                   out float4 outTexCoord    : TEXCOORD0,
                   out float  outFogStrength : TEXCOORD1,
                   out float3 outWorldPos    : TEXCOORD2,
                   out float3 outNormal      : TEXCOORD3)
{
    outPos  = mul(inPos, g_matWorldViewProj);

    // ハーフランバート
    float dot_ = 0.f;
    {
        dot_ = dot(inNormal, g_vecLightNormal);
        dot_ += 1.f;
        dot_ *= 0.5f;
    }

    float fLightIntensity = g_fLightBrigntness * dot_;

    float4 _ambient = g_vecAmbient;
    if (g_bCaveFadeFinish)
    {
        _ambient = 0.f;
    }

    outDiffuse = g_vecDiffuse * max(0, fLightIntensity) + _ambient;

    // 暗くする
    {
        outDiffuse.r *= 0.7f; 
        outDiffuse.gb *= 0.5f;
        outDiffuse.a = 1.0f;
    }

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

// 多分、赤色成分が少ないテクスチャ画像を使うと、
// 赤色部分の演算結果がオーバーフローして真っ白になる。
void pixel_shader(in float4 inDiffuse    : COLOR0,
                  in float2 inTexCoord   : TEXCOORD0,
                  in float  inFog        : TEXCOORD1,
                  in float3 inWorldPos   : TEXCOORD2,
                  in float3 inNormal     : TEXCOORD3,
                  out float4 outDiffuse  : COLOR0)
{
    float4 color_result = (float4)0;

    color_result = tex2D(mesh_texture_sampler, inTexCoord);

    outDiffuse = (inDiffuse * color_result);

    //------------------------------------------------------
    // 霧の描画
    //
    // 霧はピクセルシェーダーでやらないと意味がない。
    // 頂点シェーダーでやると、遠いほど輝いて見えるようになってしまう
    //------------------------------------------------------
    float4 fog_color2 = g_vecFogColor * g_fLightBrigntness;

    outDiffuse = (outDiffuse * (1.f - inFog)) + (fog_color2 * inFog);

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
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;

        VertexShader = compile vs_3_0 vertex_shader();
        PixelShader  = compile ps_3_0 pixel_shader();
    }

    pass PassCullNone
    {
        CullMode = None;

        VertexShader = compile vs_3_0 vertex_shader();
        PixelShader  = compile ps_3_0 pixel_shader();
    }

    // 木の葉っぱのような、
    // 反対側からも見える必要があり、
    // 完全に透明な部分（葉っぱがない部分）が存在する3Dモデルのためのパス
    pass PassLeaf
    {
        CullMode = None;
        AlphaTestEnable = TRUE;
        AlphaFunc = GreaterEqual;
        AlphaRef = 128;

        VertexShader = compile vs_3_0 vertex_shader();
        PixelShader  = compile ps_3_0 pixel_shader();
    }
}
