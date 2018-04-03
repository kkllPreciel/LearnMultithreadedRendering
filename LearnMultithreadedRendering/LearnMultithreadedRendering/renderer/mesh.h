/**
 *  @file     mesh.h
 *  @brief    ���b�V���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/02
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>
#include <Sein/Direct3D12/direct3d12_device.h>

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
     *  @brief  �I���������s��
     */
    virtual void Destroy() = 0;

    /**
     *  @brief  ���_�o�b�t�@���擾����
     *  @return ���_�o�b�t�@
     */
    virtual const Sein::Direct3D12::IVertexBuffer& GetVertexBuffer() const = 0;

    /**
     *  @brief  �C���f�b�N�X�o�b�t�@���擾����
     *  @return �C���f�b�N�X�o�b�t�@
     */
    virtual const Sein::Direct3D12::IIndexBuffer& GetIndexBuffer() const = 0;

    /**
     *  @brief  ���_�C���f�b�N�X�̌����擾����
     *  @param  ���_�C���f�b�N�X�̌�
     */
    virtual std::uint32_t GetIndexCount() const = 0;

    /**
     *  @brief  ���b�V�����쐬����
     *  @param  device:�f�o�C�X
     *  @param  vertices:���_�z��ւ̃|�C���^
     *  @param  vertex_count:���_��
     *  @param  indices:���_�C���f�b�N�X�z��ւ̃|�C���^
     *  @param  index_count:���_�C���f�b�N�X��
     *  @return ���b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IMesh> Create(Sein::Direct3D12::Device* device, void* const vertices, std::uint32_t vertex_count, void* const indices, std::uint32_t index_count);
  };
};