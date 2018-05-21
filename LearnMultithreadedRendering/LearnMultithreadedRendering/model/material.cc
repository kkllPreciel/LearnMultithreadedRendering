/**
 *  @file     material.cc
 *  @brief    �}�e���A���Ɋւ��鏈�����s���v���O�����\�[�X
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
     *  @brief  �}�e���A���p�N���X
     */
    class Material final : public IMaterial
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       *  @param  albedo:�A���x�h�}�b�v
       *  @param  roughness:���t�l�X�}�b�v
       *  @param  metallic:���^���b�N�}�b�v
       *  @param  normal:�m�[�}���}�b�v
       */
      Material(std::shared_ptr<ITexture> albedo, std::shared_ptr<ITexture> roughness, std::shared_ptr<ITexture> metallic, std::shared_ptr<ITexture> normal)
        : albedo_(albedo), roughness_(roughness), metallic_(metallic), normal_(normal)
      {

      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~Material() override
      {
        Destroy();
      }

      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {
        albedo_.reset();
        roughness_.reset();
        metallic_.reset();
        normal_.reset();
      }

    private:
      std::shared_ptr<ITexture> albedo_;    ///< �A���x�h�p�e�N�X�`��
      std::shared_ptr<ITexture> roughness_; ///< ���t�l�X�p�e�N�X�`��
      std::shared_ptr<ITexture> metallic_;  ///< ���^���b�N�p�e�N�X�`��
      std::shared_ptr<ITexture> normal_;    ///< �@���p�e�N�X�`��
    };
  };

  /**
   *  @brief  �}�e���A�����쐬����
   *  @param  albedo:�A���x�h�}�b�v
   *  @param  roughness:���t�l�X�}�b�v
   *  @param  metallic:���^���b�N�}�b�v
   *  @param  normal:�m�[�}���}�b�v
   *  @return �}�e���A���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IMaterial> IMaterial::Create(std::shared_ptr<ITexture> albedo, std::shared_ptr<ITexture> roughness, std::shared_ptr<ITexture> metallic, std::shared_ptr<ITexture> normal)
  {
    auto material = std::make_shared<Material>(albedo, roughness, metallic, normal);

    return material;
  }
};