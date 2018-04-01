/**
 *  @file     mesh.cc
 *  @brief    メッシュに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/01
 *  @version  1.0
 */

 // include
#include <DirectXMath.h>
#include "mesh.h"

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

    /**
     *  @brief  メッシュ用クラス
     */
    class Mesh final : public IMesh
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Mesh()
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~Mesh() override
      {
        Destroy();
      }

      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {

      }

    private:

    };
  };

  /**
   *  @brief  正三角形(1ポリゴン)のメッシュを作成する
   *  @param  size:中心から頂点への長さ
   *  @return メッシュ用インターフェイスへのシェアードポインタ
   */
  std::shared_ptr<IMesh> IMesh::CreateForEquilateralTriangle(const float size)
  {  
    Vertex vertices[] = {
      { { 0.0f, 0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
      { { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
      { { -0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
    };

    std::uint32_t indices[] = { 0, 1, 2 };

    auto mesh = std::make_shared<Mesh>();
    return mesh;
  }
};