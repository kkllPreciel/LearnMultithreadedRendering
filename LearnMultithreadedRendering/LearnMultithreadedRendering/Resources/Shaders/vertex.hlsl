/**
 *  @file     vertex.hlsl
 *  @brief    物理ベースレンダリング用頂点シェーダー
 *  @author   kkllPreciel
 *  @date     2018/03/20
 *  @version  1.0
 */

#include "common.hlsli"

/**
 *  @brief  頂点シェーダーの入力形式
 */
struct VSInput
{
    float3 position : POSITION; ///< 座標
    float3 normal : NORMAL;     ///< 法線ベクトル
    float2 uv : TEXCOORD;       ///< UV座標
    uint id : SV_InstanceID;    ///< インスタンスID
};

/**
 *  @brief  インスタンス毎のデータ
 */
struct InstanceBuffer
{
    float4x4 world; ///< ワールド行列
};

StructuredBuffer<InstanceBuffer> cbv : register(t0); ///< インスタンス毎のデータリスト

/**
 *  @brief  コンスタントバッファ
 */
cbuffer ConstantBuffer : register(b0)
{
    float4x4 view;          ///< ビュー行列(列優先行列)
    float4x4 projection;    ///< プロジェクション行列(列優先行列)
};

/**
 *  @brief  エントリーポイント
 */
VSOutput main(VSInput input)
{
    VSOutput result;

    float4 pos = float4(input.position, 1.0);

    pos = mul(cbv[input.id].world, pos);
    pos = mul(view, pos);
    pos = mul(projection, pos);

    result.position = pos;
    result.color = float4(1.0, 1.0, 1.0, 1.0);
    result.uv = input.uv;
    
    // ワールド空間での法線ベクトル(近似値)
    result.normal = mul(cbv[input.id].world, float4(input.normal, 1.0));

    // ワールド空間での座標
    result.wPos = mul(cbv[input.id].world, float4(input.position, 1.0));

    return result;
}