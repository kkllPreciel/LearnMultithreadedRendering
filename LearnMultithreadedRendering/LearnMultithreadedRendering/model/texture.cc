/**
 *  @file     texture.cc
 *  @brief    �e�N�X�`���Ɋւ��鏈�����s���v���O�����\�[�X
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
     *  @brief  �e�N�X�`���p�N���X
     */
    class Texture final : public ITexture
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Texture()
      {

      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~Texture() override
      {
        Destroy();
      }

      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {
        // TODO:�e�N�X�`���̍폜
      }

    private:
    };
  };

  /**
   *  @brief  �t�@�C������e�N�X�`�����쐬����
   *  @param  device:�f�o�C�X
   *  @param  file_path:�e�N�X�`���t�@�C���p�X
   *  @return �e�N�X�`���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<ITexture> CreateFromFile(Sein::Direct3D12::IDevice* const device, const std::string& file_path)
  {
    auto texture = std::make_shared<Texture>();

    // TODO:�e�N�X�`���̍쐬

    return texture;
  }
};