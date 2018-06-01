/**
 *  @file     graphics_pipeline_state.cc
 *  @brief    グラフィックスパイプラインステートに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/01
 *  @version  1.0
 */

 // include
#include "renderer/graphics_pipeline_state.h"

namespace App
{
  namespace
  {
    /**
     *  @brief  定数バッファ用クラス
     */
    class ConstantBuffer
    {
    public:
#pragma pack(push, 1)
      DirectX::XMFLOAT4X4 world_;
      DirectX::XMFLOAT4X4 view_;
      DirectX::XMFLOAT4X4 projection_;
#pragma pack(pop)
    };

    /**
     *  @brief  グラフィックスパイプラインステート用クラス
     */
    class GraphicsPipelineState final : public IGraphicsPipelineState
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      GraphicsPipelineState()
      {

      }
    
      /**
       *  @brief  デストラクタ
       */
      ~GraphicsPipelineState() override
      {
        Destroy();
      }

      /**
       *  @brief  パイプラインステートを作成する
       *  @param  device:デバイス
       */
      void Create(Sein::Direct3D12::IDevice* const device)
      {
        // ディスクリプターヒープの作成
        D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc = {};
        descriptor_heap_desc.NumDescriptors = 5;                                 // ディスクリプターヒープ内のディスクリプター数(定数バッファ、シェーダーリソース)
        descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;      // 定数バッファ or シェーダーリソース(テクスチャ) or ランダムアクセス のどれかのヒープ
        descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  // シェーダーからアクセス可
        descriptor_heap_ = device->CreateDescriptorHeap(descriptor_heap_desc);

        // 定数バッファの作成
        constant_buffer_ = device->CreateConstantBuffer(descriptor_heap_, sizeof(ConstantBuffer));
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.world_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.view_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.projection_), DirectX::XMMatrixIdentity());

        // シェーダーリソースバッファ(StructuredBuffer)の作成
        // TODO:1要素のサイズと要素数を指定できるようにする
        resource_buffer_ = device->CreateShaderResourceBuffer(descriptor_heap_, 10000, sizeof(DirectX::XMFLOAT4X4));
      }

      /**
       *  @brief  ワールド行列を複数設定する
       *  @param  world_matrices:ワールド行列配列の先頭ポインタ
       *  @param  size_in_bytes:全ワールド行列のサイズ(ワールド行列サイズ * ワールド行列数)
       */
      void SetWorldMatrices(const DirectX::XMFLOAT4X4* const world_matrices, const std::uint32_t size_in_bytes) override
      {
        resource_buffer_->Map(world_matrices, size_in_bytes);
      }

      /**
       *  @brief  ビュー行列を設定する
       *  @param  view_matrix:ビュー行列
       */
      void SetViewMatrix(const DirectX::XMFLOAT4X4& view_matrix) override
      {
        constant_buffer_instance_.view_ = view_matrix;
        constant_buffer_->Map(sizeof(ConstantBuffer), &constant_buffer_instance_);
      }
      
      /**
       *  @brief  プロジェクション行列を設定する
       *  @param  projection_matrix:プロジェクション行列
       */
      void SetProjectionMatrix(const DirectX::XMFLOAT4X4& projection_matrix) override
      {
        constant_buffer_instance_.projection_ = projection_matrix;
        constant_buffer_->Map(sizeof(ConstantBuffer), &constant_buffer_instance_);
      }

      /**
       *  @brief  ディスクリプターヒープを取得する
       *  @return ディスクリプターヒープ
       */
      std::shared_ptr<Sein::Direct3D12::IDescriptorHeap>& GetDescriptorHeap() override
      {
        return descriptor_heap_;
      }
    
      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {

      }

    private:
      std::shared_ptr<Sein::Direct3D12::IDescriptorHeap> descriptor_heap_;      ///< 定数バッファ用ディスクリプターヒープ
      ConstantBuffer constant_buffer_instance_;                                 ///< コンスタントバッファの実体
      std::unique_ptr<Sein::Direct3D12::IConstantBuffer> constant_buffer_;      ///< 定数バッファ
      std::vector<DirectX::XMFLOAT4X4> instance_buffer_;                        ///< インスタンスバッファの実体
      std::unique_ptr<Sein::Direct3D12::ShaderResourceBuffer> resource_buffer_; ///< ワールド行列用StructuredBuffer
    };
  };

  /**
   *  @brief  グラフィックスパイプラインステートを作成する
   *  @param  device:デバイス
   *  @return グラフィックスパイプラインステートインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<IGraphicsPipelineState> IGraphicsPipelineState::Create(Sein::Direct3D12::IDevice* const device)
  {
    auto graphics_pipeline_state = std::make_shared<GraphicsPipelineState>();

    graphics_pipeline_state->Create(device);

    return graphics_pipeline_state;
  }
};