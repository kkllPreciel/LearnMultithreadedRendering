/**
 *  @file     material.cc
 *  @brief    マテリアルに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/05/21
 *  @version  1.0
 */

 // include
#include "model/material.h"

namespace App
{
  namespace
  {
    /**
     *  @brief  マテリアル用クラス
     */
    class Material final : public IMaterial
    {
    public:
      /**
       *  @brief  コンストラクタ
       *  @param  albedo:アルベドマップ
       *  @param  roughness:ラフネスマップ
       *  @param  metallic:メタリックマップ
       *  @param  normal:ノーマルマップ
       */
      Material(std::shared_ptr<ITexture> albedo, std::shared_ptr<ITexture> roughness, std::shared_ptr<ITexture> metallic, std::shared_ptr<ITexture> normal)
        : albedo_(albedo), roughness_(roughness), metallic_(metallic), normal_(normal)
      {

      }
    
      /**
       *  @brief  デストラクタ
       */
      ~Material() override
      {
        Destroy();
      }

      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {
        albedo_.reset();
        roughness_.reset();
        metallic_.reset();
        normal_.reset();
      }

    private:
      std::shared_ptr<ITexture> albedo_;    ///< アルベド用テクスチャ
      std::shared_ptr<ITexture> roughness_; ///< ラフネス用テクスチャ
      std::shared_ptr<ITexture> metallic_;  ///< メタリック用テクスチャ
      std::shared_ptr<ITexture> normal_;    ///< 法線用テクスチャ
    };
  };

  /**
   *  @brief  マテリアルを作成する
   *  @param  albedo:アルベドマップ
   *  @param  roughness:ラフネスマップ
   *  @param  metallic:メタリックマップ
   *  @param  normal:ノーマルマップ
   *  @return マテリアル用インターフェイスへのシェアードポインタ
   */
  std::shared_ptr<IMaterial> IMaterial::Create(std::shared_ptr<ITexture> albedo, std::shared_ptr<ITexture> roughness, std::shared_ptr<ITexture> metallic, std::shared_ptr<ITexture> normal)
  {
    auto material = std::make_shared<Material>(albedo, roughness, metallic, normal);

    return material;
  }
};