/**
 *  @file     graphics_pipeline_state.h
 *  @brief    グラフィックスパイプラインステートに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/01
 *  @version  1.0
 */

#pragma once

// include
#include <memory>

#include <DirectXMath.h>
#include <Sein/Direct3D12/direct3d12_device.h>

namespace App
{
  /**
   *  @brief  グラフィックスパイプラインステート用インターフェイス
   */
  class IGraphicsPipelineState
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IGraphicsPipelineState() = default;
  
    /**
     *  @brief  デストラクタ
     */
    virtual ~IGraphicsPipelineState() = default;
  
    /**
     *  @brief  終了処理を行う
     */
    virtual void Destroy() = 0;
  
    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IGraphicsPipelineState(const IGraphicsPipelineState& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    IGraphicsPipelineState& operator = (const IGraphicsPipelineState& other) = delete;

    /**
     *  @brief  ワールド行列を複数設定する
     *  @param  world_matrices:ワールド行列配列の先頭ポインタ
     *  @param  size_in_bytes:全ワールド行列のサイズ(ワールド行列サイズ * ワールド行列数)
     */
    virtual void SetWorldMatrices(const DirectX::XMFLOAT4X4* const world_matrices, const std::uint32_t size_in_bytes) = 0;

    /**
     *  @brief  ビュー行列を設定する
     *  @param  view_matrix:ビュー行列
     */
    virtual void SetViewMatrix(const DirectX::XMFLOAT4X4& view_matrix) = 0;

    /**
     *  @brief  プロジェクション行列を設定する
     *  @param  projection_matrix:プロジェクション行列
     */
    virtual void SetProjectionMatrix(const DirectX::XMFLOAT4X4& projection_matrix) = 0;

    /**
     *  @brief  ディスクリプターヒープを取得する
     *  @return ディスクリプターヒープ
     */
    virtual std::shared_ptr<Sein::Direct3D12::IDescriptorHeap>& GetDescriptorHeap() = 0;

    /**
     *  @brief  グラフィックスパイプラインステートを作成する
     *  @param  device:デバイス
     *  @return グラフィックスパイプラインステートインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IGraphicsPipelineState> Create(Sein::Direct3D12::IDevice* const device);
  };
};