/**
 *  @file     mesh_loader.cc
 *  @brief    メッシュローダーに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/05/10
 *  @version  1.0
 */

 // include
#include "mesh_loader.h"
#include <vector>
#include <fstream>
#include <cassert>
#include <DirectXMath.h>

namespace App
{
  namespace
  {
    // アラインメントを1バイトに設定
#pragma pack(push, 1)
    struct Vertex
    {
      DirectX::XMFLOAT3 position; ///< 座標
      DirectX::XMFLOAT3 normal;   ///< 法線ベクトル
      DirectX::XMFLOAT2 texcoord; ///< テクスチャUV座標
    };
#pragma pack(pop)

    /**
     *  @brief  メッシュデータ用クラス
     */
    class MeshData final : public IMeshData
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      MeshData() : vertices_({}), indices_({})
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~MeshData()
      {
        vertices_.clear();
        indices_.clear();
      }

      /**
       *  @brief  1頂点のサイズ(バイト数)を取得する
       *  @return 1頂点のサイズ
       */
      std::uint32_t GetOneVertexSize() override
      {
        // TODO:動的に変更できるようにする
        return static_cast<std::uint32_t>(sizeof(Vertex));
      }
      
      /**
       *  @brief  全頂点合計のサイズ(バイト数)を取得する
       *  @return 全頂点のサイズ
       */
      std::uint32_t GetAllVertexSize() override
      {
        assert(sizeof(Vertex) * vertices_.size() < UINT32_MAX);

        // TODO:動的に変更できるようにする
        return static_cast<std::uint32_t>(sizeof(Vertex) * vertices_.size());
      }
      
      /**
       *  @brief  1頂点インデックスのサイズ(バイト数)を取得する
       *  @return 1頂点インデックスのサイズ
       */
      std::uint32_t GetOneVertexIndexSize() override
      {
        // TODO:動的に変更できるようにする
        return static_cast<std::uint32_t>(sizeof(std::uint32_t));
      }
      
      /**
       *  @brief  全頂点インデックス合計のサイズ(バイト数)を取得する
       *  @return 全頂点インデックス合計のサイズ
       */
      std::uint32_t GetAllVertexIndexSize() override
      {
        assert(sizeof(std::uint32_t) * indices_.size() < UINT32_MAX);

        // TODO:動的に変更できるようにする
        return static_cast<std::uint32_t>(sizeof(std::uint32_t) * indices_.size());
      }
      
      /**
       *  @brief  頂点インデックス数を取得する
       *  @return 頂点インデックス数
       */
      std::uint32_t GetVertexIndexCount() override
      {
        return static_cast<std::uint32_t>(indices_.size());
      }

      /**
       *  @brief  頂点データ配列の先頭ポインタを取得する
       *  @return 頂点データ配列の先頭ポインタ
       */
      void* GetVertexData() override
      {
        // TODO:型を「void*」からtemplateに変更する
        return static_cast<void*>(vertices_.data());
      }
      
      /**
       *  @brief  頂点インデックスデータ配列の先頭ポインタを取得する
       *  @return 頂点インデックスデータ配列の先頭ポインタ
       */
      void* GetVertexIndexData() override
      {
        // TODO:型を「void*」からtemplateに変更する
        return static_cast<void*>(indices_.data());
      }

    private:
      // TODO:頂点データタイプ(座標、UV座標、法線)
      // TODO:頂点インデックスタイプ
      std::vector<Vertex> vertices_;        ///< 頂点の配列
      std::vector<std::uint32_t> indices_;  ///< 頂点インデックスの配列
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
        // TODO:拡張子チェック

        std::ifstream stream;
        stream.open(file_path, std::ifstream::in | std::ifstream::binary);
        if (false == stream.operator bool())
        {
          throw std::exception("ファイルのオープンに失敗しました。");
        }

        // 読み込み実行
        std::istreambuf_iterator<char> it(stream);
        std::istreambuf_iterator<char> last;
        std::string data(it, last);
        stream.close();

        auto mesh_data = std::make_shared<MeshData>();

//        std::unique_ptr<Model> model = nullptr;
//
//
//        // C4996警告を無効にする
//#if defined(_MSC_VER) && _MSC_VER >= 1400
//#pragma warning(push)
//#pragma warning(disable:4996)
//#endif
//        // 行毎に分割する
//        std::vector<std::string> list;
//        boost::split(list, data, boost::is_any_of("\r\n"));
//#if defined(_MSC_VER) && _MSC_VER >= 1400
//#pragma warning(pop)
//#endif
//
//        model = std::make_unique<Model>();
//
//        // モデルデータ読み込み(行単位で処理を行う)
//        for (auto line : list)
//        {
//          std::vector<std::string> parts;
//          boost::split(parts, line, boost::is_space());
//
//          // 分割出来なかった(違法なデータ)
//          if (parts.size() <= 1)
//          {
//            continue;
//          }
//
//          // キーワードを取得し対応する処理を行う
//          switch (GetID(parts.at(0)))
//          {
//            // コメント
//          case ObjKeywords::kComment:
//            continue;
//            break;
//            // マテリアルファイル名
//          case ObjKeywords::kMaterialFileName:
//            continue;
//            break;
//            // グループ
//          case ObjKeywords::kGroup:
//            continue;
//            break;
//            // 現在のグループデータが使用するマテリアル名
//          case ObjKeywords::kUseMaterial:
//            break;
//            // 頂点座標
//          case ObjKeywords::kControlPoint:
//            // TODO:要素数が4でない場合は違法データとして戻す
//            model->AddControlPoint(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
//            break;
//            // テクスチャ座標
//          case ObjKeywords::kVertexTexture:
//            // TODO:要素数が3でない場合は違法データとして戻す
//            model->AddTextureCoord(DirectX::XMFLOAT2(std::stof(parts[1]), std::stof(parts[2])));
//            break;
//            // 頂点法線ベクトル
//          case ObjKeywords::kVertexNormal:
//            // TODO:要素数が4でない場合は違法データとして戻す
//            model->AddNormal(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
//            break;
//            // ポリゴンデータ
//          case ObjKeywords::kPolygon:
//          {
//            std::vector<uint32_t> point_indices;
//            std::vector<uint32_t> normal_indices;
//            std::vector<uint32_t> texture_coord_indices;
//            for (decltype(parts.size()) i = 1; i < parts.size(); ++i)
//            {
//              std::vector<std::string> indices;
//              boost::split(indices, parts[i], boost::is_any_of("/"));
//              for (auto index : indices | boost::adaptors::indexed())
//              {
//                if (false == index.value().empty())
//                {
//                  // 座標・テクスチャ座標・法線ベクトルは0番目からなので、-1を行う
//                  const auto value = std::stoi(index.value()) - 1;
//                  switch (index.index())
//                  {
//                  case 0:
//
//                    point_indices.emplace_back(value);
//                    break;
//                  case 1:
//                    texture_coord_indices.emplace_back(value);
//                    break;
//                  case 2:
//                    normal_indices.emplace_back(value);
//                    break;
//                  }
//                }
//              }
//            }
//            model->AddPointIndex(point_indices);
//            model->AddTextureCoordIndex(texture_coord_indices);
//            model->AddNormalIndex(normal_indices);
//            model->AddPolygon();
//          }
//          break;
//          // 不明キーワード
//          default:
//            continue;
//            break;
//          }
//        }
//
//        return model;

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