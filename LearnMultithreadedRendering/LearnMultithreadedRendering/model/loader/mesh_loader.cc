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
     *  @brief  ���b�V���f�[�^�p�N���X
     */
    class MeshData final : public IMeshData
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      MeshData()
      {

      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~MeshData()
      {

      }

      /**
       *  @brief  1���_�̃T�C�Y(�o�C�g��)���擾����
       *  @return 1���_�̃T�C�Y
       */
      std::uint32_t GetOneVertexSize() override
      {
        // TODO:�������T�C�Y�ɏC������
        return 0;
      }
      
      /**
       *  @brief  �S���_���v�̃T�C�Y(�o�C�g��)���擾����
       *  @return �S���_�̃T�C�Y
       */
      std::uint32_t GetAllVertexSize() override
      {
        // TODO:�������T�C�Y�ɏC������
        return 0;
      }
      
      /**
       *  @brief  1���_�C���f�b�N�X�̃T�C�Y(�o�C�g��)���擾����
       *  @return 1���_�C���f�b�N�X�̃T�C�Y
       */
      std::uint32_t GetOneVertexIndexSize() override
      {
        // TODO:�������T�C�Y�ɏC������
        return 0;
      }
      
      /**
       *  @brief  �S���_�C���f�b�N�X���v�̃T�C�Y(�o�C�g��)���擾����
       *  @return �S���_�C���f�b�N�X���v�̃T�C�Y
       */
      std::uint32_t GetAllVertexIndexSize() override
      {
        // TODO:�������T�C�Y�ɏC������
        return 0;
      }
      
      /**
       *  @brief  ���_�C���f�b�N�X�����擾����
       *  @return ���_�C���f�b�N�X��
       */
      std::uint32_t GetVertexIndexCount() override
      {
        // TODO:�������T�C�Y�ɏC������
        return 0;
      }
      
      /**
       *  @brief  ���_�o�b�t�@�ƒ��_�C���f�b�N�X�o�b�t�@�Ƀ}�b�v����
       *  @param  vertex_buffer:���_�o�b�t�@
       *  @param  index_buffer:���_�C���f�b�N�X�o�b�t�@
       */
      void Map(std::shared_ptr<Sein::Direct3D12::IVertexBuffer>& vertex_buffer, std::shared_ptr<Sein::Direct3D12::IIndexBuffer>& index_buffer) override
      {
        // TODO:�}�b�v���s��
      }

    private:
      // ���_�f�[�^�^�C�v(���W�AUV���W�A�@��)
      // ���_�C���f�b�N�X�f�[�^�^�C�v(uint32_t�Ȃ���Ȃ��C������)
      // ���_�f�[�^
      // ���_�C���f�b�N�X�f�[�^
    };

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
        auto mesh_data = std::make_shared<MeshData>();

        // TODO:�ǂݍ��ݏ���

        return mesh_data;
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