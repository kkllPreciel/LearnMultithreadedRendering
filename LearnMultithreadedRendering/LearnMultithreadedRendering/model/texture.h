/**
 *  @file     texture.h
 *  @brief    テクスチャに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/05/21
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <string>
#include <Sein/Direct3D12/direct3d12_device.h>

namespace App
{
  /**
   *  @brief  テクスチャ用インターフェイス
   */
  class ITexture
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    ITexture() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~ITexture() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    ITexture(const ITexture& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    ITexture& operator = (const ITexture& other) = delete;

    /**
     *  @brief  終了処理を行う
     */
    virtual void Destroy() = 0;

    // TODO:テクスチャの適用 or 取得インターフェイス

    /**
     *  @brief  ファイルからテクスチャを作成する
     *  @param  device:デバイス
     *  @param  file_path:テクスチャファイルパス
     *  @return テクスチャ用インターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<ITexture> CreateFromFile(Sein::Direct3D12::IDevice* const device, const std::string& file_path);
  };
};