/**
 *  @file     material.h
 *  @brief    �}�e���A���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/05/21
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include "model/texture.h"

namespace App
{
  /**
   *  @brief  �}�e���A��(PBR)�p�C���^�[�t�F�C�X
   */
  class IMaterial
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    IMaterial() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IMaterial() = default;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    IMaterial(const IMaterial& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    IMaterial& operator = (const IMaterial& other) = delete;

    /**
     *  @brief  �I���������s��
     */
    virtual void Destroy() = 0;

    // �A���x�h�e�N�X�`���̎擾?(�K�p)
    // ���t�l�X�e�N�X�`���̎擾?(�K�p)
    // ���^���b�N�e�N�X�`���̎擾?(�K�p)
    // �@���e�N�X�`���̎擾?(�K�p)

    /**
     *  @brief  �}�e���A�����쐬����
     *  @param  albedo:�A���x�h�}�b�v
     *  @param  roughness:���t�l�X�}�b�v
     *  @param  metallic:���^���b�N�}�b�v
     *  @param  normal:�m�[�}���}�b�v
     *  @return �}�e���A���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IMaterial> Create(std::shared_ptr<ITexture> albedo, std::shared_ptr<ITexture> roughness, std::shared_ptr<ITexture> metallic, std::shared_ptr<ITexture> normal);
  };
};