// BOMありのUTF8だとコンパイルできなくなる。そのため、シェーダーファイルだけはBOMなし

float4x4 g_matWorld;
float4x4 g_matWorldViewProj;

float4 g_vecLightNormal;
float g_fLightBrigntness;

float4 g_vecDiffuse;
float4 g_vecAmbient = { 0.2f, 0.1f, 0.1f, 0.0f };

texture g_texture;
sampler g_samplerMeshTexture = sampler_state
{
    Texture   = (g_texture);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//-----------------------------------------------------
// 頂点シェーダー
//-----------------------------------------------------
void vertex_shader(in  float4 inPos        : POSITION,
                   in  float4 inNormal     : NORMAL0,
                   in  float4 inTexCoord   : TEXCOORD0,
                   out float4 outPos       : POSITION,
                   out float4 outDiffuse   : COLOR0,
                   out float4 outTexCoord  : TEXCOORD0)
{
    outPos  = mul(inPos, g_matWorldViewProj);

    //-----------------------------------------------------
    // ハーフランバート、という方法で陰影をつける。
    // 通常のランバート拡散照明は、光源の方角とモデルの面の法線の角度に応じて陰影をつけるが、
    // このやり方だと、球に光を当てた場合、光が当たっていない側のすべて黒になる。
    // ハーフランバートは、これに対処するために、光源と法線が真逆の時に０になるようにする。
    // 物理的には正しくない。
    //-----------------------------------------------------

    float dot_ = 0.f;

    {
        // 名前がややこしいが、法線ベクトルを正規化
        inNormal = normalize(inNormal);

        dot_ = dot(inNormal, g_vecLightNormal);
        dot_ += 1.f;
        dot_ *= 0.5f;
    }

    // 現在の頂点における、あてられている光の強さ
    float fLightIntensity = g_fLightBrigntness * dot_;

    // 色 * 光の強さ + 環境光
    outDiffuse = g_vecDiffuse * max(0, fLightIntensity) + g_vecAmbient;

    // 0.0 ~ 1.0の範囲に収まるようにする
    outDiffuse = saturate(outDiffuse);

    outTexCoord = inTexCoord;
}

//-----------------------------------------------------
// ピクセルシェーダー
//-----------------------------------------------------
void pixel_shader(in  float4 inDiffuse     : COLOR0,
                  in  float2 inTexCoord    : TEXCOORD0,
                  out float4 outDiffuse    : COLOR0)
{
    float4 color_result = (float4)0;
    color_result = tex2D(g_samplerMeshTexture, inTexCoord);
    outDiffuse = (inDiffuse * color_result);
}

technique Technique1
{
    pass Pass1
    {
        VertexShader = compile vs_3_0 vertex_shader();
        PixelShader  = compile ps_3_0 pixel_shader();
    }
}

