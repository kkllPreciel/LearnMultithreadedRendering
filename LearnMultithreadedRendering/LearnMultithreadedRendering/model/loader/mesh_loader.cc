/**
 *  @file     mesh_loader.cc
 *  @brief    メッシュローダーに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/05/10
 *  @version  1.0
 */

 // include
#include "mesh_loader.h"

namespace App
{
  namespace
  {
    /**
     *  @brief  メッシュ読み込み用クラス
     */
    class MeshLoader final : public IMeshLoader
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      MeshLoader()
      {

      }
    
      /**
       *  @brief  デストラクタ
       */
      ~MeshLoader() override
      {
        Destroy();
      }

      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {

      }

      /**
       *  @brief  OBJフォーマットファイルからメッシュデータを読み込む
       *  @param  file_path:読み込みを行うファイルのパス
       *  @return メッシュデータインターフェイスへのポインタ
       */
      std::shared_ptr<IMeshData> LoadFromObj(const std::string& file_path) override
      {
        return nullptr;
      }

    private:
    };
  };

  /**
   *  @brief  メッシュローダーを作成する
   *  @return メッシュローダーインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<IMeshLoader> IMeshLoader::Create()
  {
    auto mesh_loader = std::make_shared<MeshLoader>();

    return mesh_loader;
  }
};