/**
 *  @file     mesh_loader.cc
 *  @brief    ���b�V�����[�_�[�Ɋւ��鏈�����s���v���O�����\�[�X
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
     *  @brief  ���b�V���ǂݍ��ݗp�N���X
     */
    class MeshLoader final : public IMeshLoader
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      MeshLoader()
      {

      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~MeshLoader() override
      {
        Destroy();
      }

      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {

      }

      /**
       *  @brief  OBJ�t�H�[�}�b�g�t�@�C�����烁�b�V���f�[�^��ǂݍ���
       *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
       *  @return ���b�V���f�[�^�C���^�[�t�F�C�X�ւ̃|�C���^
       */
      std::shared_ptr<IMeshData> LoadFromObj(const std::string& file_path) override
      {
        return nullptr;
      }

    private:
    };
  };

  /**
   *  @brief  ���b�V�����[�_�[���쐬����
   *  @return ���b�V�����[�_�[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IMeshLoader> IMeshLoader::Create()
  {
    auto mesh_loader = std::make_shared<MeshLoader>();

    return mesh_loader;
  }
};