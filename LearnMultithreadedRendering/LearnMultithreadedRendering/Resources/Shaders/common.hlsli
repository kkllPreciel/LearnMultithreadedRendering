/**
 *  @file     common.hlsli
 *  @brief    物理ベースレンダリング用共通シェーダー
 *  @author   kkllPreciel
 *  @date     2018/03/20
 *  @version  1.0
 */

/**
 *  @brief  頂点シェーダーの出力形式
 */
struct VSOutput
{
    float4 position : SV_POSITION;  ///< 座標
    float4 wPos : COLOR0;           ///< ワールド空間での座標
    float4 normal : COLOR1;         ///< ワールド空間での法線ベクトル
    float2 uv : TEXCOORD;           ///< UV座標
    float4 color : COLOR2;          ///< 色
};