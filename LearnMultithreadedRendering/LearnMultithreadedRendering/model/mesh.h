/**
 *  @file     mesh.h
 *  @brief    ���b�V���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/31
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>

namespace App
{
  /**
   *  @brief  ���b�V���p�C���^�[�t�F�C�X
   */
  class IMesh
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    IMesh() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IMesh() = default;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    IMesh(const IMesh& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    IMesh& operator = (const IMesh& other) = delete;

    /**
     *  @brief  ���_�o�b�t�@���擾����
     *  @return ���_�o�b�t�@
     */
    virtual const Sein::Direct3D12::VertexBuffer& GetVertexBuffer() const = 0;

    /**
     *  @brief  �C���f�b�N�X�o�b�t�@���擾����
     *  @return �C���f�b�N�X�o�b�t�@
     */
    virtual const Sein::Direct3D12::IndexBuffer& GetIndexBuffer() const = 0;

    /**
     *  @brief  ���_�C���f�b�N�X�̌����擾����
     *  @param  ���_�C���f�b�N�X�̌�
     */
    virtual std::uint32_t GetIndexCount() const = 0;

    /**
     *  @brief  ���O�p�`(1�|���S��)�̃��b�V�����쐬����
     *  @param  size:���S���璸�_�ւ̒���
     *  @return ���b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IMesh> CreateForEquilateralTriangle(const float size);

    // �|��
    // ����������
  };
};