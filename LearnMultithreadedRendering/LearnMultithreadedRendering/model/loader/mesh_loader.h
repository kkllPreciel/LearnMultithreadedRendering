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
#include "../mesh.h"
#include "../renderer/renderer.h"

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

    // 1���_�̃T�C�Y
    virtual std::uint32_t GetOneVertexSize() = 0;

    // �S���_���v�̃T�C�Y
    virtual std::uint32_t GetAllVertexSize() = 0;

    // 1���_�C���f�b�N�X�̃T�C�Y
    virtual std::uint32_t GetOneVertexIndexSize() = 0;

    // �S���_�C���f�b�N�X�̃T�C�Y
    virtual std::uint32_t GetAllVertexIndexSize() = 0;

    // ���_�R�s�[�֐�
    // ���_�C���f�b�N�X�R�s�[�֐�
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