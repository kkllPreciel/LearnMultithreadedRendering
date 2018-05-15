/**
 *  @file     mesh_loader.h
 *  @brief    ���b�V�����[�_�[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/05/10
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <string>

namespace App
{
  /**
   *  @brief  ���b�V���f�[�^�p�C���^�[�t�F�C�X
   */
  class IMeshData
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    IMeshData() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IMeshData() = default;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    IMeshData(const IMeshData& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    IMeshData& operator = (const IMeshData& other) = delete;

    /**
     *  @brief  1���_�̃T�C�Y(�o�C�g��)���擾����
     *  @return 1���_�̃T�C�Y
     */
    virtual std::uint32_t GetOneVertexSize() = 0;

    /**
     *  @brief  �S���_���v�̃T�C�Y(�o�C�g��)���擾����
     *  @return �S���_�̃T�C�Y
     */
    virtual std::uint32_t GetAllVertexSize() = 0;

    /**
     *  @brief  1���_�C���f�b�N�X�̃T�C�Y(�o�C�g��)���擾����
     *  @return 1���_�C���f�b�N�X�̃T�C�Y
     */
    virtual std::uint32_t GetOneVertexIndexSize() = 0;

    /**
     *  @brief  �S���_�C���f�b�N�X���v�̃T�C�Y(�o�C�g��)���擾����
     *  @return �S���_�C���f�b�N�X���v�̃T�C�Y
     */
    virtual std::uint32_t GetAllVertexIndexSize() = 0;

    /**
     *  @brief  ���_�C���f�b�N�X�����擾����
     *  @return ���_�C���f�b�N�X��
     */
    virtual std::uint32_t GetVertexIndexCount() = 0;

    /**
     *  @brief  ���_�f�[�^�z��̐擪�|�C���^���擾����
     *  @return ���_�f�[�^�z��̐擪�|�C���^
     */
    virtual void* GetVertexData() = 0;

    /**
     *  @brief  ���_�C���f�b�N�X�f�[�^�z��̐擪�|�C���^���擾����
     *  @return ���_�C���f�b�N�X�f�[�^�z��̐擪�|�C���^
     */
    virtual void* GetVertexIndexData() = 0;
  };

  /**
   *  @brief  ���b�V�����[�_�[�p�C���^�[�t�F�C�X
   */
  class IMeshLoader
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    IMeshLoader() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IMeshLoader() = default;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    IMeshLoader(const IMeshLoader& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    IMeshLoader& operator = (const IMeshLoader& other) = delete;

    /**
     *  @brief  �I���������s��
     */
    virtual void Destroy() = 0;

    /**
     *  @brief  OBJ�t�H�[�}�b�g�t�@�C�����烁�b�V���f�[�^��ǂݍ���
     *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
     *  @return ���b�V���f�[�^�C���^�[�t�F�C�X�ւ̃|�C���^
     */
    virtual std::shared_ptr<IMeshData> LoadFromObj(const std::string& file_path) = 0;

    /**
     *  @brief  ���b�V�����[�_�[���쐬����
     *  @return ���b�V�����[�_�[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IMeshLoader> Create();
  };
};