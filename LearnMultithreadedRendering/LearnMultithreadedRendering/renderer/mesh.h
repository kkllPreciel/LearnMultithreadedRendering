/**
 *  @file     mesh.h
 *  @brief    メッシュに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/02
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>
#include <Sein/Direct3D12/direct3d12_device.h>

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
     *  @brief  頂点バッファを取得する
     *  @return 頂点バッファ
     */
    virtual const Sein::Direct3D12::IVertexBuffer& GetVertexBuffer() const = 0;

    /**
     *  @brief  インデックスバッファを取得する
     *  @return インデックスバッファ
     */
    virtual const Sein::Direct3D12::IIndexBuffer& GetIndexBuffer() const = 0;

    /**
     *  @brief  頂点インデックスの個数を取得する
     *  @param  頂点インデックスの個数
     */
    virtual std::uint32_t GetIndexCount() const = 0;

    /**
     *  @brief  メッシュを作成する
     *  @param  device:デバイス
     *  @param  vertices:頂点配列へのポインタ
     *  @param  vertex_count:頂点数
     *  @param  indices:頂点インデックス配列へのポインタ
     *  @param  index_count:頂点インデックス数
     *  @return メッシュ用インターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IMesh> Create(Sein::Direct3D12::Device* device, void* const vertices, std::uint32_t vertex_count, void* const indices, std::uint32_t index_count);
  };
};