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
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>
#include "../mesh.h"

namespace App
{
  /**
   *  @brief  メッシュデータ用インターフェイス
   */
  class IMeshData
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IMeshData() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~IMeshData() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IMeshData(const IMeshData& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    IMeshData& operator = (const IMeshData& other) = delete;

    /**
     *  @brief  1頂点のサイズ(バイト数)を取得する
     *  @return 1頂点のサイズ
     */
    virtual std::uint32_t GetOneVertexSize() = 0;

    /**
     *  @brief  全頂点合計のサイズ(バイト数)を取得する
     *  @return 全頂点のサイズ
     */
    virtual std::uint32_t GetAllVertexSize() = 0;

    /**
     *  @brief  1頂点インデックスのサイズ(バイト数)を取得する
     *  @return 1頂点インデックスのサイズ
     */
    virtual std::uint32_t GetOneVertexIndexSize() = 0;

    /**
     *  @brief  全頂点インデックス合計のサイズ(バイト数)を取得する
     *  @return 全頂点インデックス合計のサイズ
     */
    virtual std::uint32_t GetAllVertexIndexSize() = 0;

    /**
     *  @brief  頂点インデックス数を取得する
     *  @return 頂点インデックス数
     */
    virtual std::uint32_t GetVertexIndexCount() = 0;

    /**
     *  @brief  頂点バッファと頂点インデックスバッファにマップする
     *  @param  vertex_buffer:頂点バッファ
     *  @param  index_buffer:頂点インデックスバッファ
     */
    virtual void Map(std::shared_ptr<Sein::Direct3D12::IVertexBuffer>& vertex_buffer, std::shared_ptr<Sein::Direct3D12::IIndexBuffer>& index_buffer) = 0;
  };

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
     *  @brief  OBJフォーマットファイルからメッシュデータを読み込む
     *  @param  file_path:読み込みを行うファイルのパス
     *  @return メッシュデータインターフェイスへのポインタ
     */
    virtual std::shared_ptr<IMeshData> LoadFromObj(const std::string& file_path) = 0;

    /**
     *  @brief  メッシュローダーを作成する
     *  @return メッシュローダーインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IMeshLoader> Create();
  };
};