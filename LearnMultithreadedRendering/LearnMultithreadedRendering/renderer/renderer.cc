/**
 *  @file     renderer.cc
 *  @brief    レンダラーに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/03/17
 *  @version  1.0
 */

 // include
#include <Sein/Direct3D12/direct3d12_device.h>
#include "renderer/renderer.h"

namespace App
{
  namespace
  {
    /**
     *  @brief  レンダラー用クラス
     */
    class Renderer final : public IRenderer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Renderer() : device_(nullptr), execute_command_list_(nullptr), store_command_list_(nullptr)
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
      }

      /**
       *  @brief  実行する
       */
      void Execute() override
      {
        // 実行用コマンドリストと作成用コマンドリストを交換する
        execute_command_list_.swap(store_command_list_);

        // オブジェクトの描画に必要なもの
        // ワールド座標
        // カメラ
        // プロジェクション
        // メッシュ
        // ライト
        // ボーン行列

        // 前フレームで作成したコマンドのリストを実行する(GPU)
        device_->ExecuteCommandLists(execute_command_list_.get());

        // TODO:前フレームのゲーム情報からコマンドのリストを作成する(キューに格納されている予定)

        // TODO:BeginScene、EndScene内のリソース指定を変更できるように
        auto buffer_index = device_->GetNextBackBufferIndex();
        device_->BeginScene(store_command_list_.get(), buffer_index);
        device_->EndScene(store_command_list_.get(), buffer_index);
      }
      
      /**
       *  @brief  画面を更新する
       */
      void Present() override
      {
        // TODO:描画終了待ちを行う

        // 画面の更新を行う
        device_->Present();
      }
    
      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {
        store_command_list_.reset();
        execute_command_list_.reset();
        device_.reset();
      }

    private:
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