/**
 *  @file     renderer.cc
 *  @brief    レンダラーに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/03/17
 *  @version  1.0
 */

 // include
#include <thread>
#include <atomic>
#include <mutex>
#include <Sein/Direct3D12/direct3d12_device.h>
#include "renderer/renderer.h"

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
      Renderer() : device_(nullptr), execute_command_list_(nullptr), store_command_list_(nullptr),
        thread_(nullptr), terminate_(false), processing_(false),
        execute_render_object_list_(nullptr), store_render_object_list_(nullptr)
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
       */
      void Create(HWND handle, std::uint32_t width, std::uint32_t height)
      {
        assert(device_ == nullptr);
        device_ = std::make_unique<Sein::Direct3D12::Device>();
        device_->Create(handle, width, height);

        // コマンドリストの作成
        execute_command_list_ = device_->CreateCommandList(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
        store_command_list_ = device_->CreateCommandList(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);

        // レンダーオブジェクトキューの作成
        execute_render_object_list_ = std::make_unique<std::vector<RenderObject>>();
        store_render_object_list_ = std::make_unique<std::vector<RenderObject>>();

        // スレッドの作成
        terminate_ = false;
        processing_ = false;
        thread_ = std::make_unique<std::thread>(&Renderer::ThreadMain, this);

        // 定数バッファの作成
        constant_buffer_ = device_->CreateConstantBuffer(sizeof(ConstantBuffer));
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.world_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.view_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.projection_), DirectX::XMMatrixIdentity());
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
        // TODO:ビューの登録
      }

      /**
       *  @brief  実行する
       */
      void Execute() override
      {
        // レンダリングスレッドを起動する
        processing_ = true;
        condition_.notify_all();
      }
      
      /**
       *  @brief  画面を更新する
       */
      void Present() override
      {
        // レンダリングスレッドの処理の終了待ちを行う
        // ここに来るまでに処理が終了している可能性がある
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [&] {return false == processing_; });

        // TODO:描画終了待ちを行う

        // 画面の更新を行う
        device_->Present();
      }
    
      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {
        // レンダリングスレッドの終了待ちを行う
        if (thread_ && thread_->joinable())
        {
          terminate_ = true;
          condition_.notify_all();
          thread_->join();
          thread_.reset();
        }

        store_render_object_list_.reset();
        execute_render_object_list_.reset();

        store_command_list_.reset();
        execute_command_list_.reset();
        device_.reset();
      }

    private:
      /**
       *  @brief  スレッドのメイン関数
       */
      void ThreadMain()
      {
        // 終了フラグが立っていなければ実行
        while (terminate_ == false)
        {
          // スレッドの待機
          std::unique_lock<std::mutex> lock(mutex_);
          condition_.wait(lock, [&] { return processing_ || terminate_; });

          if (terminate_)
          {
            continue;
          }

          // 実行用コマンドリストと作成用コマンドリストを交換する
          execute_command_list_.swap(store_command_list_);

          // 前フレームで作成したコマンドのリストを実行する(GPU)
          device_->ExecuteCommandLists(execute_command_list_.get());

          // 実行用描画オブジェクトのキューと作成用描画オブジェクトのキューを交換する
          execute_render_object_list_.swap(store_render_object_list_);

          // TODO:BeginScene、EndScene内のリソース指定を変更できるように
          auto buffer_index = device_->GetNextBackBufferIndex();
          device_->BeginScene(store_command_list_.get(), buffer_index);

          // TODO:前フレームのゲーム情報からコマンドのリストを作成する(キューに格納されている予定)

          // TODO:ドローコールバッチング、ダイナミックバッチング
          // TODO:draw in direct

          // ドローコール
          for (auto& render_object : *execute_render_object_list_)
          {
            auto& vertex_buffer = const_cast<Sein::Direct3D12::IVertexBuffer&>(render_object.vertex_buffer_);
            auto& index_buffer = const_cast<Sein::Direct3D12::IIndexBuffer&>(render_object.index_buffer_);
            auto index_count = render_object.index_count_;
            constant_buffer_instance_.world_ = render_object.matrix_;
            constant_buffer_->Map(sizeof(ConstantBuffer), &(constant_buffer_instance_));
            store_command_list_->SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            store_command_list_->SetVertexBuffers(0, 1, &(vertex_buffer.GetView()));
            store_command_list_->SetIndexBuffer(&(index_buffer.GetView()));
            device_->Render(store_command_list_.get(), index_count, 1);
          }

          device_->EndScene(store_command_list_.get(), buffer_index);

          // オブジェクトの描画に必要なもの
          // ワールド座標
          // カメラ
          // プロジェクション
          // メッシュ
          // ライト
          // ボーン行列

          // スレッドの処理終了通知
          processing_ = false;
          condition_.notify_all();
        }
      }

    private:
      std::unique_ptr<std::thread> thread_;                                   ///< スレッド
      std::atomic<bool> terminate_;                                           ///< 終了フラグ
      bool processing_;                                                       ///< 処理中フラグ
      std::mutex mutex_;                                                      ///< スレッド間排他処理用
      std::condition_variable condition_;                                     ///< スレッド間実行待機用
      std::unique_ptr<Sein::Direct3D12::Device> device_;                      ///< デバイス
      std::shared_ptr<Sein::Direct3D12::ICommandList> execute_command_list_;  ///< 実行用コマンドリスト
      std::shared_ptr<Sein::Direct3D12::ICommandList> store_command_list_;    ///< 作成用コマンドリスト
      std::unique_ptr<std::vector<RenderObject>> execute_render_object_list_; ///< 実行用描画オブジェクトのリスト
      std::unique_ptr<std::vector<RenderObject>> store_render_object_list_;   ///< 作成用描画オブジェクトのリスト


      ConstantBuffer constant_buffer_instance_;                               ///< コンスタントバッファの実体
      std::unique_ptr<Sein::Direct3D12::IConstantBuffer> constant_buffer_;    ///< 定数バッファ
    };
  };

  /**
   *  @brief  レンダラーを作成する
   *  @param  handle:ウィンドウハンドル
   *  @param  width:ウィンドウの横幅
   *  @param  height:ウィンドウの縦幅
   *  @return レンダラーインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<IRenderer> IRenderer::Create(HWND handle, std::uint32_t width, std::uint32_t height)
  {
    auto renderer = std::make_unique<Renderer>();

    renderer->Create(handle, width, height);

    return renderer;
  }
};