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


// TODO:削除
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>

namespace App
{
  namespace
  {
    // TODO:削除
    // アラインメントを1バイトに設定
#pragma pack(push, 1)
    struct Vertex
    {
      DirectX::XMFLOAT3 position; ///< 座標
      DirectX::XMFLOAT3 normal;   ///< 法線ベクトル
      DirectX::XMFLOAT2 texcoord; ///< テクスチャUV座標
    };
#pragma pack(pop)
    static Sein::Direct3D12::VertexBuffer vertex_buffer;
    static Sein::Direct3D12::IndexBuffer index_buffer;

    /**
     *  @brief  レンダラー用クラス
     */
    class Renderer final : public IRenderer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Renderer() : device_(nullptr), execute_command_list_(nullptr), store_command_list_(nullptr), thread_(nullptr), terminate_(false), processing_(false)
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

        // スレッドの作成
        terminate_ = false;
        processing_ = false;
        thread_ = std::make_unique<std::thread>(&Renderer::ThreadMain, this);


        // TODO削除
        Vertex vertices[] = {
          { { 0.0f, 0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
          { { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
          { { -0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
        };
        std::uint32_t indices[] = { 0, 1, 2 };
        vertex_buffer.Create(const_cast<ID3D12Device*>(&device_->GetDevice()), sizeof(Vertex) * 3, sizeof(Vertex), &vertices);
        index_buffer.Create(const_cast<ID3D12Device*>(&device_->GetDevice()), sizeof(std::uint32_t) * 3, &indices, DXGI_FORMAT_R32_UINT);
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

          // TODO:前フレームのゲーム情報からコマンドのリストを作成する(キューに格納されている予定)

          // オブジェクトの描画に必要なもの
          // ワールド座標
          // カメラ
          // プロジェクション
          // メッシュ
          // ライト
          // ボーン行列

          // TODO:BeginScene、EndScene内のリソース指定を変更できるように
          auto buffer_index = device_->GetNextBackBufferIndex();
          device_->BeginScene(store_command_list_.get(), buffer_index);

          // TODO:削除
          store_command_list_->SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
          store_command_list_->SetVertexBuffers(0, 1, &(vertex_buffer.GetView()));
          store_command_list_->SetIndexBuffer(&(index_buffer.GetView()));
          device_->Render(store_command_list_.get(), 3, 1);

          device_->EndScene(store_command_list_.get(), buffer_index);


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