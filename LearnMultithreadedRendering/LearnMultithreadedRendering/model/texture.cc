/**
 *  @file     texture.cc
 *  @brief    テクスチャに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/05/21
 *  @version  1.0
 */

 // include
#include "texture.h"

namespace App
{
  namespace
  {
    /**
     *  @brief  テクスチャ用クラス
     */
    class Texture final : public ITexture
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Texture()
      {

      }
    
      /**
       *  @brief  デストラクタ
       */
      ~Texture() override
      {
        Destroy();
      }

      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {
        // TODO:テクスチャの削除
      }

    private:
    };
  };

  /**
   *  @brief  ファイルからテクスチャを作成する
   *  @param  device:デバイス
   *  @param  file_path:テクスチャファイルパス
   *  @return テクスチャ用インターフェイスへのシェアードポインタ
   */
  std::shared_ptr<ITexture> CreateFromFile(Sein::Direct3D12::IDevice* const device, const std::string& file_path)
  {
    auto texture = std::make_shared<Texture>();

    // TODO:テクスチャの作成

    return texture;
  }
};