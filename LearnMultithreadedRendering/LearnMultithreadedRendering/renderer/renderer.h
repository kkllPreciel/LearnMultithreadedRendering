/**
 *  @file     renderer.h
 *  @brief    レンダラーに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/17
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
   *  @brief  レンダラー用インターフェイス
   */
  class IRenderer
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IRenderer() = default;
  
    /**
     *  @brief  デストラクタ
     */
    virtual ~IRenderer() = default;

    /**
     *  @brief  実行する
     */
    virtual void Execute() = 0;

    /**
     *  @brief  頂点バッファを作成する
     *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
     *  @return 頂点バッファへのユニークポインタ
     */
    virtual std::unique_ptr<Sein::Direct3D12::IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes) = 0;
    
    /**
     *  @brief  頂点インデックスバッファを作成する
     *  @param  size_in_bytes:頂点インデックスバッファのサイズ(頂点インデックスサイズ * 頂点インデックス数)
     *  @return 頂点インデックスバッファのユニークID
     */
    virtual std::unique_ptr<Sein::Direct3D12::IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes) = 0;

    /**
     *  @brief  画面を更新する
     */
    virtual void Present() = 0;
  
    /**
     *  @brief  終了処理を行う
     */
    virtual void Destroy() = 0;
  
    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IRenderer(const IRenderer& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    IRenderer& operator = (const IRenderer& other) = delete;

    /**
     *  @brief  レンダラーを作成する
     *  @param  handle:ウィンドウハンドル
     *  @param  width:ウィンドウの横幅
     *  @param  height:ウィンドウの縦幅
     *  @return レンダラーインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IRenderer> Create(HWND handle, std::uint32_t width, std::uint32_t height);
  };
};