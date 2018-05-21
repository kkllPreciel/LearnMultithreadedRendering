/**
 *  @file     material.h
 *  @brief    マテリアルに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/05/21
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include "model/texture.h"

namespace App
{
  /**
   *  @brief  マテリアル(PBR)用インターフェイス
   */
  class IMaterial
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IMaterial() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~IMaterial() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IMaterial(const IMaterial& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    IMaterial& operator = (const IMaterial& other) = delete;

    /**
     *  @brief  終了処理を行う
     */
    virtual void Destroy() = 0;

    // アルベドテクスチャの取得?(適用)
    // ラフネステクスチャの取得?(適用)
    // メタリックテクスチャの取得?(適用)
    // 法線テクスチャの取得?(適用)

    /**
     *  @brief  マテリアルを作成する
     *  @param  albedo:アルベドマップ
     *  @param  roughness:ラフネスマップ
     *  @param  metallic:メタリックマップ
     *  @param  normal:ノーマルマップ
     *  @return マテリアル用インターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IMaterial> Create(std::shared_ptr<ITexture> albedo, std::shared_ptr<ITexture> roughness, std::shared_ptr<ITexture> metallic, std::shared_ptr<ITexture> normal);
  };
};