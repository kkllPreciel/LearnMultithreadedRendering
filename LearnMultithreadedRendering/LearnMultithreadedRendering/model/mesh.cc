/**
 *  @file     mesh.cc
 *  @brief    ���b�V���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/01
 *  @version  1.0
 */

 // include
#include <DirectXMath.h>
#include "mesh.h"

namespace App
{
  namespace
  {
    // TODO:�폜
    // �A���C�������g��1�o�C�g�ɐݒ�
#pragma pack(push, 1)
    struct Vertex
    {
      DirectX::XMFLOAT3 position; ///< ���W
      DirectX::XMFLOAT3 normal;   ///< �@���x�N�g��
      DirectX::XMFLOAT2 texcoord; ///< �e�N�X�`��UV���W
    };
#pragma pack(pop)

    /**
     *  @brief  ���b�V���p�N���X
     */
    class Mesh final : public IMesh
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Mesh()
      {

      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~Mesh() override
      {
        Destroy();
      }

      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {

      }

    private:

    };
  };

  /**
   *  @brief  ���O�p�`(1�|���S��)�̃��b�V�����쐬����
   *  @param  size:���S���璸�_�ւ̒���
   *  @return ���b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IMesh> IMesh::CreateForEquilateralTriangle(const float size)
  {  
    Vertex vertices[] = {
      { { 0.0f, 0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
      { { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
      { { -0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
    };

    std::uint32_t indices[] = { 0, 1, 2 };

    auto mesh = std::make_shared<Mesh>();
    return mesh;
  }
};