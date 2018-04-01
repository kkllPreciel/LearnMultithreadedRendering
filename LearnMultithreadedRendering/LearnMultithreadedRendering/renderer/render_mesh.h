/**
 *  @file     render_mesh.h
 *  @brief    描画用メッシュに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/01
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
   *  @brief  描画用メッシュ用インターフェイス
   */
  class IRenderMesh
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IRenderMesh() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~IRenderMesh() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IRenderMesh(const IRenderMesh& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    IRenderMesh& operator = (const IRenderMesh& other) = delete;

    /**
     *  @brief  終了処理を行う
     */
    virtual void Destroy() = 0;

    /**
     *  @brief  頂点バッファを取得する
     *  @return 頂点バッファ
     */
    virtual const Sein::Direct3D12::VertexBuffer& GetVertexBuffer() const = 0;

    /**
     *  @brief  インデックスバッファを取得する
     *  @return インデックスバッファ
     */
    virtual const Sein::Direct3D12::IndexBuffer& GetIndexBuffer() const = 0;

    /**
     *  @brief  頂点インデックスの個数を取得する
     *  @param  頂点インデックスの個数
     */
    virtual std::uint32_t GetIndexCount() const = 0;

    /**
     *  @brief  描画用のメッシュを作成する
     *  @param  device:デバイス
     *  @param  vertices:頂点配列へのポインタ
     *  @param  vertex_count:頂点数
     *  @param  indices:頂点インデックス配列へのポインタ
     *  @param  index_count:頂点インデックス数
     *  @return 描画用のメッシュ用インターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IRenderMesh> Create(Sein::Direct3D12::Device* device, void* const vertices, std::uint32_t vertex_count, void* const indices, std::uint32_t index_count);
  };
};