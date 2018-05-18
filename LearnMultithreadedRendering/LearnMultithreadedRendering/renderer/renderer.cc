/**
 *  @file     renderer.cc
 *  @brief    レンダラーに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/03/17
 *  @version  1.0
 */

 // include
#include "renderer/renderer.h"
#include <Sein/Direct3D12/direct3d12_device.h>
#include <Sein/Direct3D12/shader_resource_buffer.h>
#include "renderer/double_command_list.h"
#include "task/task.h" 
#include "task/task_group.h" 

namespace App
{
  namespace
  {
    /**
     *  @brief  描画オブジェクト用クラス
     */
    class RenderObject
    {
    public:
      RenderObject(const Sein::Direct3D12::IVertexBuffer& vertex_buffer, const Sein::Direct3D12::IIndexBuffer& index_buffer, const std::uint32_t index_count, const DirectX::XMFLOAT4X4& matrix)
        : vertex_buffer_(vertex_buffer), index_buffer_(index_buffer), index_count_(index_count), matrix_(matrix)
      {

      }

      const Sein::Direct3D12::IVertexBuffer& vertex_buffer_;
      const Sein::Direct3D12::IIndexBuffer& index_buffer_;
      const std::uint32_t index_count_;
      const DirectX::XMFLOAT4X4& matrix_;
    };

    /**
     *  @brief  ビュー用クラス
     */
    class View
    {
    public:
      std::uint32_t view_id_;
      DirectX::XMFLOAT4X4 view_matrix_;
      DirectX::XMFLOAT4X4 projection_matrix_;
      D3D12_VIEWPORT viewport_;
      D3D12_RECT scissor_;
    };

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
     *  @brief  レンダラー用クラス
     */
    class Renderer final : public IRenderer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Renderer() : device_(nullptr), double_command_list_(nullptr),
        execute_render_object_list_(nullptr), store_render_object_list_(nullptr),
        scheduler_(nullptr), task_group_(nullptr)
      {

      }
    
      /**
       *  @brief  デストラクタ
       */
      ~Renderer() override
      {
        Destroy();
      }

      /**
       *  @brief  デバイスを作成する
       *  @param  handle:ウィンドウハンドル
       *  @param  width:ウィンドウの横幅
       *  @param  height:ウィンドウの縦幅
       *  @param  scheduler:タスクスケジューラ
       */
      void Create(HWND handle, std::uint32_t width, std::uint32_t height, std::shared_ptr<ITaskScheduler> scheduler)
      {
        assert(device_ == nullptr);
        device_ = std::make_unique<Sein::Direct3D12::Device>();
        device_->Create(handle, width, height);

        assert(scheduler_ == nullptr);
        scheduler_ = scheduler;

        // コマンドリストの作成
        double_command_list_ = IDoubleCommandList::Create(device_.get(), D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);

        // レンダーオブジェクトキューの作成
        execute_render_object_list_ = std::make_unique<std::vector<RenderObject>>();
        store_render_object_list_ = std::make_unique<std::vector<RenderObject>>();

        // 定数バッファの作成
        constant_buffer_ = device_->CreateConstantBuffer(sizeof(ConstantBuffer));
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.world_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.view_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.projection_), DirectX::XMMatrixIdentity());

        // シェーダーリソースバッファ(StructuredBuffer)の作成
        resource_buffer_ = device_->CreateShaderResourceBuffer(10000, sizeof(DirectX::XMFLOAT4X4));

        // ビューの作成
        view_ = std::make_unique<View>();
        view_->view_id_ = 1;
        DirectX::XMStoreFloat4x4(&(view_->view_matrix_), DirectX::XMMatrixRotationZ(DirectX::XM_PI / 2));
        DirectX::XMStoreFloat4x4(&(view_->projection_matrix_), DirectX::XMMatrixIdentity());
      }

      /**
       *  @brief  頂点バッファを作成する
       *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
       *  @return 頂点バッファへのユニークポインタ
       */
      std::unique_ptr<Sein::Direct3D12::IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes) override
      {
        return device_->CreateVertexBuffer(size_in_bytes);
      }
      
      /**
       *  @brief  頂点インデックスバッファを作成する
       *  @param  size_in_bytes:頂点インデックスバッファのサイズ(頂点インデックスサイズ * 頂点インデックス数)
       *  @return 頂点インデックスバッファのユニークID
       */
      std::unique_ptr<Sein::Direct3D12::IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes) override
      {
        return device_->CreateIndexBuffer(size_in_bytes);
      }

      /**
       *  @brief  描画オブジェクトの登録を行う
       *  @param  vertex_buffer:描画オブジェクトが使用する頂点バッファ
       *  @param  index_buffer:描画オブジェクトが使用する頂点インデックスバッファ
       *  @param  index_count:頂点インデックスの個数
       *  @param  matrix:描画オブジェクトのワールド空間行列
       */
      void Register(const Sein::Direct3D12::IVertexBuffer& vertex_buffer, const Sein::Direct3D12::IIndexBuffer& index_buffer, const std::uint32_t index_count, const DirectX::XMFLOAT4X4& matrix) override
      {
        // TODO:swapのタイミングで実行されても問題ないように
        store_render_object_list_->emplace_back(RenderObject(vertex_buffer, index_buffer, index_count, matrix));
      }

      /**
       *  @brief  ビューを登録する
       *  @param  view_id:ビューのID
       *  @param  view_matrix:ビュー行列
       *  @param  projection_matrix:プロジェクション行列
       *  @param  viewport:ビューポート矩形の設定
       *  @param  scissor:シザー矩形の設定
       */
      void RegisterView(const std::uint32_t view_id, const DirectX::XMFLOAT4X4& view_matrix, const DirectX::XMFLOAT4X4& projection_matrix, const D3D12_VIEWPORT viewport, const D3D12_RECT scissor) override
      {
        assert(!view_);

        // ビューの作成
        view_->view_id_ = view_id;
        view_->view_matrix_ = view_matrix;
        view_->projection_matrix_ = projection_matrix;
        view_->viewport_ = viewport;
        view_->scissor_ = scissor;
      }

      /**
       *  @brief  実行する
       */
      void Execute() override
      {
        assert(task_group_ == nullptr);
        assert(scheduler_ != nullptr);

        // タスクの登録
        task_group_ = ITaskGroup::Create(std::vector<std::shared_ptr<ITask>>({ ITask::Create([&](std::uint64_t delta_time) { ThreadMain(); }) }), std::vector<std::shared_ptr<ITaskGroup>>());
        scheduler_->Register(task_group_);
      }
      
      /**
       *  @brief  画面を更新する
       */
      void Present() override
      {
        // タスクの終了待ち
        // ここに来るまでに処理が終了している可能性がある
        task_group_->Wait();
        task_group_.reset();

        // TODO:描画終了待ちを行う

        // 画面の更新を行う
        device_->Present();
      }
    
      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {
        store_render_object_list_.reset();
        execute_render_object_list_.reset();

        double_command_list_.reset();
        device_.reset();
      }

    private:
      /**
       *  @brief  スレッドのメイン関数
       */
      void ThreadMain()
      {
        // 実行用コマンドリストと作成用コマンドリストを交換する
        double_command_list_->Swap();

        // 前フレームで作成したコマンドのリストを実行する(GPU)
        device_->ExecuteCommandLists(const_cast<Sein::Direct3D12::ICommandList*>(&(double_command_list_->GetFrontCommandList())));

        decltype(auto) store_command_list = double_command_list_->GetBackCommandList();

        // 実行用描画オブジェクトのキューと作成用描画オブジェクトのキューを交換する
        execute_render_object_list_.swap(store_render_object_list_);
        store_render_object_list_->clear();

        // 描画するオブジェクトが存在しない
        if (execute_render_object_list_->size() <= 0)
        {
          return;
        }

        // TODO:BeginScene、EndScene内のリソース指定を変更できるように
        auto buffer_index = device_->GetNextBackBufferIndex();
        device_->BeginScene(&store_command_list, buffer_index);

        // TODO:前フレームのゲーム情報からコマンドのリストを作成する(キューに格納されている予定)

        // TODO:ドローコールバッチング、ダイナミックバッチング
        // TODO:draw in direct

        // 定数バッファの設定(ビュー、プロジェクション)
        constant_buffer_instance_.view_ = view_->view_matrix_;
        constant_buffer_instance_.projection_ = view_->projection_matrix_;
        constant_buffer_->Map(sizeof(ConstantBuffer), &(constant_buffer_instance_));

        // シェーダーリソースバッファの設定(ワールド)
        instance_buffer_.clear();
        for (auto& render_object : *execute_render_object_list_)
        {
          instance_buffer_.emplace_back(render_object.matrix_);
        }
        resource_buffer_->Map(instance_buffer_.data(), sizeof(DirectX::XMFLOAT4X4) * 10000);

        auto& vertex_buffer = const_cast<Sein::Direct3D12::IVertexBuffer&>(execute_render_object_list_->at(0).vertex_buffer_);
        auto& index_buffer = const_cast<Sein::Direct3D12::IIndexBuffer&>(execute_render_object_list_->at(0).index_buffer_);
        auto index_count = execute_render_object_list_->at(0).index_count_;
        store_command_list.SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        store_command_list.SetVertexBuffers(0, 1, &(vertex_buffer.GetView()));
        store_command_list.SetIndexBuffer(&(index_buffer.GetView()));
        device_->Render(&store_command_list, index_count, 10000);

        // TODO:ビューポートの設定
        // TODO:シザー矩形の設定

        device_->EndScene(&store_command_list, buffer_index);

        // オブジェクトの描画に必要なもの
        // ワールド座標
        // カメラ
        // プロジェクション
        // メッシュ
        // ライト
        // ボーン行列
      }

    private:
      std::unique_ptr<Sein::Direct3D12::Device> device_;                      ///< デバイス
      std::shared_ptr<App::IDoubleCommandList> double_command_list_;          ///< コマンドリスト(ダブルバッファリングする)

      std::unique_ptr<std::vector<RenderObject>> execute_render_object_list_; ///< 実行用描画オブジェクトのリスト
      std::unique_ptr<std::vector<RenderObject>> store_render_object_list_;   ///< 作成用描画オブジェクトのリスト


      ConstantBuffer constant_buffer_instance_;                               ///< コンスタントバッファの実体
      std::unique_ptr<Sein::Direct3D12::IConstantBuffer> constant_buffer_;    ///< 定数バッファ
      std::vector<DirectX::XMFLOAT4X4> instance_buffer_;                      ///< インスタンスバッファの実体
      std::unique_ptr<Sein::Direct3D12::ShaderResourceBuffer> resource_buffer_;

      std::unique_ptr<View> view_;                                            ///< ビュー

      std::shared_ptr<ITaskScheduler> scheduler_;                             ///< スケジューラ
      std::shared_ptr<ITaskGroup> task_group_;                                ///< タスクグループ
    };
  };

  /**
   *  @brief  レンダラーを作成する
   *  @param  handle:ウィンドウハンドル
   *  @param  width:ウィンドウの横幅
   *  @param  height:ウィンドウの縦幅
   *  @param  scheduler:タスクスケジューラ
   *  @return レンダラーインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<IRenderer> IRenderer::Create(HWND handle, std::uint32_t width, std::uint32_t height, std::shared_ptr<ITaskScheduler> scheduler)
  {
    auto renderer = std::make_unique<Renderer>();

    renderer->Create(handle, width, height, scheduler);

    return renderer;
  }
};