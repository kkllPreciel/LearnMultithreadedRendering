/**
 *  @file     mesh.h
 *  @brief    メッシュに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/31
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>

namespace App
{
  /**
   *  @brief  メッシュ用インターフェイス
   */
  class IMesh
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IMesh() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~IMesh() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IMesh(const IMesh& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    IMesh& operator = (const IMesh& other) = delete;

    /**
     *  @brief  終了処理を行う
     */
    virtual void Destroy() = 0;

    /**
     *  @brief  正三角形(1ポリゴン)のメッシュを作成する
     *  @param  size:中心から頂点への長さ
     *  @return メッシュ用インターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IMesh> CreateForEquilateralTriangle(const float size);

    // 板ポリ
    // メモリから
  };
};