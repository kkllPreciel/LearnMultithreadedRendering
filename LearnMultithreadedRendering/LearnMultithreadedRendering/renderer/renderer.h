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
#include <DirectXMath.h>
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
     *  @brief  描画オブジェクトの登録を行う
     *  @param  vertex_buffer:描画オブジェクトが使用する頂点バッファ
     *  @param  index_buffer:描画オブジェクトが使用する頂点インデックスバッファ
     *  @param  index_count:頂点インデックスの個数
     *  @param  matrix:描画オブジェクトのワールド空間行列
     */
    virtual void Register(const Sein::Direct3D12::IVertexBuffer& vertex_buffer, const Sein::Direct3D12::IIndexBuffer& index_buffer, const std::uint32_t index_count, const DirectX::XMFLOAT4X4& matrix) = 0;

    /**
     *  @brief  ビューを登録する
     *  @param  view_id:ビューのID
     *  @param  view_matrix:ビュー行列
     *  @param  projection_matrix:プロジェクション行列
     *  @param  viewport:ビューポート矩形の設定
     *  @param  scissor:シザー矩形の設定
     */
    virtual void RegisterView(const std::uint32_t view_id, const DirectX::XMFLOAT4X4& view_matrix, const DirectX::XMFLOAT4X4& projection_matrix, const D3D12_VIEWPORT viewport, const D3D12_RECT scissor) = 0;

    /**
     *  @brief  実行する
     */
    virtual void Execute() = 0;

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