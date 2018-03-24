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
      Renderer() : device_(nullptr)
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
      }
    
      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {
        device_.reset();
      }

    private:
      std::unique_ptr<Sein::Direct3D12::Device> device_;  ///< デバイス
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