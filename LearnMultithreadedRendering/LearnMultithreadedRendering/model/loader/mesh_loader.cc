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
     *  @brief  メッシュデータ用クラス
     */
    class MeshData final : public IMeshData
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      MeshData()
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~MeshData()
      {

      }

      /**
       *  @brief  1頂点のサイズ(バイト数)を取得する
       *  @return 1頂点のサイズ
       */
      std::uint32_t GetOneVertexSize() override
      {
        // TODO:正しいサイズに修正する
        return 0;
      }
      
      /**
       *  @brief  全頂点合計のサイズ(バイト数)を取得する
       *  @return 全頂点のサイズ
       */
      std::uint32_t GetAllVertexSize() override
      {
        // TODO:正しいサイズに修正する
        return 0;
      }
      
      /**
       *  @brief  1頂点インデックスのサイズ(バイト数)を取得する
       *  @return 1頂点インデックスのサイズ
       */
      std::uint32_t GetOneVertexIndexSize() override
      {
        // TODO:正しいサイズに修正する
        return 0;
      }
      
      /**
       *  @brief  全頂点インデックス合計のサイズ(バイト数)を取得する
       *  @return 全頂点インデックス合計のサイズ
       */
      std::uint32_t GetAllVertexIndexSize() override
      {
        // TODO:正しいサイズに修正する
        return 0;
      }
      
      /**
       *  @brief  頂点インデックス数を取得する
       *  @return 頂点インデックス数
       */
      std::uint32_t GetVertexIndexCount() override
      {
        // TODO:正しいサイズに修正する
        return 0;
      }
      
      /**
       *  @brief  頂点バッファと頂点インデックスバッファにマップする
       *  @param  vertex_buffer:頂点バッファ
       *  @param  index_buffer:頂点インデックスバッファ
       */
      void Map(std::shared_ptr<Sein::Direct3D12::IVertexBuffer>& vertex_buffer, std::shared_ptr<Sein::Direct3D12::IIndexBuffer>& index_buffer) override
      {
        // TODO:マップを行う
      }

    private:
      // 頂点データタイプ(座標、UV座標、法線)
      // 頂点インデックスデータタイプ(uint32_tなら問題ない気がする)
      // 頂点データ
      // 頂点インデックスデータ
    };

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
        auto mesh_data = std::make_shared<MeshData>();

        // TODO:読み込み処理

        return mesh_data;
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