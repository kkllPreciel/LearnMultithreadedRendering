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

#include <../MeshLoader/MeshLoader/mesh_loader.h>

#include "../renderer/renderer.h"
#include "../model/material.h"

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
     *  @return ���_�C���f�b�N�X�̌�
     */
    virtual std::uint32_t GetIndexCount() const = 0;

    /**
     *  @brief  �}�e���A�����擾����
     *  @return �}�e���A��
     */
    virtual std::shared_ptr<IMaterial> GetMaterial() const = 0;

    /**
     *  @brief  ���b�V�����쐬����
     *  @param  renderer:�����_���[
     *  @param  vertices:���_�z��ւ̃|�C���^
     *  @param  vertex_count:���_��
     *  @param  indices:���_�C���f�b�N�X�z��ւ̃|�C���^
     *  @param  index_count:���_�C���f�b�N�X��
     *  @return ���b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IMesh> Create(IRenderer* const renderer, void* const vertices, std::uint32_t vertex_count, void* const indices, std::uint32_t index_count);

    /**
     *  @brief  ���b�V���f�[�^���烁�b�V�����쐬����
     *  @param  renderer:�����_���[
     *  @param  mesh_data:���b�V���f�[�^
     *  @return ���b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IMesh> CreateFromMeshData(IRenderer* const renderer, std::shared_ptr<MeshLoader::IMeshData> mesh_data);

    /**
     *  @brief  �O�p�`(1�|���S��)�̃��b�V�����쐬����
     *  @param  renderer:�����_���[
     *  @param  positions:���_���W�̔z��
     *  @return ���b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IMesh> CreateForTriangle(IRenderer* const renderer, DirectX::XMFLOAT3 positions[3]);
  };
};