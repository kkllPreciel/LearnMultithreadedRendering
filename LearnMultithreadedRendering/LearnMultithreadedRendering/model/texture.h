/**
 *  @file     texture.h
 *  @brief    �e�N�X�`���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/05/21
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <string>
#include "../renderer/renderer.h"

namespace App
{
  /**
   *  @brief  �e�N�X�`���p�C���^�[�t�F�C�X
   */
  class ITexture
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    ITexture() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~ITexture() = default;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    ITexture(const ITexture& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    ITexture& operator = (const ITexture& other) = delete;

    /**
     *  @brief  �I���������s��
     */
    virtual void Destroy() = 0;

    // TODO:�e�N�X�`���̓K�p or �擾�C���^�[�t�F�C�X

    /**
     *  @brief  �t�@�C������e�N�X�`�����쐬����
     *  @param  renderer:�����_���[
     *  @param  file_path:�e�N�X�`���t�@�C���p�X
     *  @return �e�N�X�`���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<ITexture> CreateFromFile(std::shared_ptr<IRenderer> renderer, const std::string& file_path);
  };
};