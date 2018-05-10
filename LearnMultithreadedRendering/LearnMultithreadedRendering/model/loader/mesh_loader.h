/**
 *  @file     mesh_loader.h
 *  @brief    メッシュローダーに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/05/10
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include "../mesh.h"

namespace App
{
  /**
   *  @brief  メッシュローダー用インターフェイス
   */
  class IMeshLoader
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IMeshLoader() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~IMeshLoader() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IMeshLoader(const IMeshLoader& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    IMeshLoader& operator = (const IMeshLoader& other) = delete;

    /**
     *  @brief  終了処理を行う
     */
    virtual void Destroy() = 0;

    /**
     *  @brief  OBJフォーマットファイルからメッシュを読み込む
     *  @param  file_path:読み込みを行うファイルのパス
     *  @return メッシュインターフェイスへのポインタ
     */
    virtual std::shared_ptr<IMesh> LoadFromObj(const std::string& file_path) = 0;

    /**
     *  @brief  メッシュローダーを作成する
     *  @return メッシュローダーインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IMeshLoader> Create();
  };
};