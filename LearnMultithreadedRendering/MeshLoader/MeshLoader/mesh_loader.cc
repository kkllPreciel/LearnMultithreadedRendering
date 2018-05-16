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
#include <map>
#include <unordered_map>

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <DirectXMath.h>

namespace MeshLoader
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

    enum ObjKeywords {
      kIllegal = 0,
      kComment,
      kMaterialFileName,
      kGroup,
      kUseMaterial,
      kControlPoint,
      kVertexTexture,
      kVertexNormal,
      kPolygon,
    };

    ObjKeywords GetID(std::string keyword)
    {
      std::map<std::string, ObjKeywords> map =
      {
        { "#", ObjKeywords::kComment },
        { "mtllib", ObjKeywords::kMaterialFileName },
        { "g", ObjKeywords::kGroup },
        { "usemtl", ObjKeywords::kUseMaterial },
        { "v", ObjKeywords::kControlPoint },
        { "vt", ObjKeywords::kVertexTexture },
        { "vn", ObjKeywords::kVertexNormal },
        { "f", ObjKeywords::kPolygon },
      };

      if (map.count(keyword) == 0)
      {
        return ObjKeywords::kIllegal;
      }

      return map.at(keyword);
    }

    /**
     *  @brief  メッシュデータ用クラス
     */
    class MeshData final : public IMeshData
    {
    public:
      /**
       *  @brief  コンストラクタ
       *  @param  vertices:頂点データ配列
       *  @param  indices:頂点インデックスデータ配列
       */
      MeshData(std::vector<Vertex>& vertices, std::vector<std::uint32_t>& indices) : vertices_(vertices), indices_(indices)
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
       *  @brief  頂点数を取得する
       *  @return 頂点数
       */
      std::uint32_t GetVertexCount() override
      {
        return static_cast<std::uint32_t>(vertices_.size());
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

        // 行毎に分割する
        std::vector<std::string> list;
        boost::split(list, data, boost::is_any_of("\r\n"));

        // 頂点データ
        std::vector<DirectX::XMFLOAT3> control_points;
        std::vector<DirectX::XMFLOAT2> texture_coords;
        std::vector<DirectX::XMFLOAT3> normals;

        // 頂点インデックスデータ
        std::vector<uint32_t> point_indices;
        std::vector<uint32_t> normal_indices;
        std::vector<uint32_t> texture_coord_indices;

        // メッシュデータ読み込み(行単位で処理を行う)
        for (auto line : list)
        {
          std::vector<std::string> parts;
          boost::split(parts, line, boost::is_space());

          // 分割出来なかった(違法なデータ)
          if (parts.size() <= 1)
          {
            continue;
          }

          // キーワードを取得し対応する処理を行う
          switch (GetID(parts.at(0)))
          {
            // コメント
          case ObjKeywords::kComment:
            continue;
            break;
            // マテリアルファイル名
          case ObjKeywords::kMaterialFileName:
            continue;
            break;
            // グループ
          case ObjKeywords::kGroup:
            continue;
            break;
            // 現在のグループデータが使用するマテリアル名
          case ObjKeywords::kUseMaterial:
            break;
            // 頂点座標
          case ObjKeywords::kControlPoint:
            // TODO:要素数が4でない場合は違法データとして戻す
            control_points.emplace_back(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
            break;
            // テクスチャ座標
          case ObjKeywords::kVertexTexture:
            // TODO:要素数が3でない場合は違法データとして戻す
            texture_coords.emplace_back(DirectX::XMFLOAT2(std::stof(parts[1]), std::stof(parts[2])));
            break;
            // 頂点法線ベクトル
          case ObjKeywords::kVertexNormal:
            // TODO:要素数が4でない場合は違法データとして戻す
            normals.emplace_back(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
            break;
            // ポリゴンデータ
          case ObjKeywords::kPolygon:
          {
            for (decltype(parts.size()) i = 1; i < parts.size(); ++i)
            {
              std::vector<std::string> indices;
              boost::split(indices, parts[i], boost::is_any_of("/"));
              for (auto index : indices | boost::adaptors::indexed())
              {
                if (false == index.value().empty())
                {
                  // 座標・テクスチャ座標・法線ベクトルは0番目からなので、-1を行う
                  const auto value = std::stoi(index.value()) - 1;
                  switch (index.index())
                  {
                  case 0:
                    point_indices.emplace_back(value);
                    break;
                  case 1:
                    texture_coord_indices.emplace_back(value);
                    break;
                  case 2:
                    normal_indices.emplace_back(value);
                    break;
                  }
                }
              }
            }
          }
          break;
          // 不明キーワード
          default:
            continue;
            break;
          }
        }
        
        // 座標・UV座標・法線ベクトルから頂点データを生成する
        std::vector<Vertex> vertices;
        std::unordered_map<std::string, uint32_t> map;
        std::vector<uint32_t> indices;

        // 頂点データリストと、頂点データインデックスとハッシュを持つリストを作成する
        for (const auto point : point_indices | boost::adaptors::indexed())
        {
          const auto index = point.index();

          // ハッシュを作成
          std::string hash = std::to_string(point.value());

          if (texture_coord_indices.empty() == false)
          {
            hash += '-' + std::to_string(texture_coord_indices[index]);
          }

          if (normal_indices.empty() == false)
          {
            hash += '-' + std::to_string(normal_indices[index]);
          }

          // ハッシュが存在しない場合は頂点データを追加する
          if (map.count(hash) == 0)
          {
            // 頂点データ
            Vertex vertex = {};
            vertex.position = control_points.at(point.value());

            if (normal_indices.empty() == false)
            {
              vertex.normal = normals.at(normal_indices.at(index));
            }

            if (texture_coord_indices.empty() == false)
            {
              vertex.texcoord = texture_coords.at(texture_coord_indices.at(index));
            }

            vertices.emplace_back(vertex);

            // ハッシュリストを更新
            map.insert({ hash, static_cast<uint32_t>(vertices.size() - 1) });
          }

          // インデックスリストにインデックスを追加する
          indices.emplace_back(map.at(hash));
        }

        auto mesh_data = std::make_shared<MeshData>(vertices, indices);

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