/**
 *  @file     mesh.cc
 *  @brief    ���b�V���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/02
 *  @version  1.0
 */

 // include
#include <DirectXMath.h>
#include "mesh.h"

namespace App
{
  namespace
  {
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
      Mesh() : vertex_buffer_(nullptr), index_buffer_(nullptr), vertex_count_(0), index_count_(0)
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
        vertex_buffer_.reset();
        index_buffer_.reset();
        vertex_count_ = 0;
        index_count_ = 0;
      }

      /**
       *  @brief  ���_�o�b�t�@��ݒ肷��
       *  @param  vertex_buffer:���_�o�b�t�@
       *  @param  vertex_count:���_��
       */
      void SetVertexBuffer(Sein::Direct3D12::IVertexBuffer* const vertex_buffer, const std::uint32_t vertex_count)
      {
        vertex_buffer_.reset(vertex_buffer);
        vertex_count_ = vertex_count;
      }

      /**
       *  @brief  �C���f�b�N�X�o�b�t�@��ݒ肷��
       *  @param  index_buffer:�C���f�b�N�o�b�t�@
       *  @param  index_count:�C���f�b�N�X��
       */
      void SetIndexBuffer(Sein::Direct3D12::IIndexBuffer* const index_buffer, const std::uint32_t index_count)
      {
        index_buffer_.reset(index_buffer);
        index_count_ = index_count;
      }
      
      /**
       *  @brief  ���_�o�b�t�@���擾����
       *  @return ���_�o�b�t�@
       */
      const Sein::Direct3D12::IVertexBuffer& GetVertexBuffer() const override
      {
        return *vertex_buffer_;
      }
      
      /**
       *  @brief  �C���f�b�N�X�o�b�t�@���擾����
       *  @return �C���f�b�N�X�o�b�t�@
       */
      const Sein::Direct3D12::IIndexBuffer& GetIndexBuffer() const override
      {
        return *index_buffer_;
      }
      
      /**
       *  @brief  ���_�C���f�b�N�X�̌����擾����
       *  @return ���_�C���f�b�N�X�̌�
       */
      std::uint32_t GetIndexCount() const override
      {
        return index_count_;
      }

    private:
      std::unique_ptr<Sein::Direct3D12::IVertexBuffer> vertex_buffer_;  ///< ���_�o�b�t�@
      std::unique_ptr<Sein::Direct3D12::IIndexBuffer> index_buffer_;    ///< �C���f�b�N�X�o�b�t�@
      std::uint32_t vertex_count_;                                      ///< ���_��
      std::uint32_t index_count_;                                       ///< �C���f�b�N�X��
    };
  };

  /**
   *  @brief  ���b�V�����쐬����
   *  @param  renderer:�����_���[
   *  @param  vertices:���_�z��ւ̃|�C���^
   *  @param  vertex_count:���_��
   *  @param  indices:���_�C���f�b�N�X�z��ւ̃|�C���^
   *  @param  index_count:���_�C���f�b�N�X��
   *  @return ���b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IMesh> IMesh::Create(IRenderer* const renderer, void* const vertices, std::uint32_t vertex_count, void* const indices, std::uint32_t index_count)
  {
    auto mesh = std::make_shared<Mesh>();
    auto vertex_buffer = renderer->CreateVertexBuffer(sizeof(Vertex) * vertex_count).release();
    auto index_buffer = renderer->CreateIndexBuffer(sizeof(std::uint32_t) * index_count).release();

    mesh->SetVertexBuffer(vertex_buffer, vertex_count);
    mesh->SetIndexBuffer(index_buffer, index_count);

    vertex_buffer->Map(sizeof(Vertex), vertices);
    index_buffer->Map(DXGI_FORMAT_R32_UINT, indices);

    return mesh;
  }

  /**
   *  @brief  ���b�V���f�[�^���烁�b�V�����쐬����
   *  @param  renderer:�����_���[
   *  @param  mesh_data:���b�V���f�[�^
   *  @return ���b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IMesh> IMesh::CreateFromMeshData(IRenderer* const renderer, std::shared_ptr<MeshLoader::IMeshData> mesh_data)
  {
    auto vertex_buffer = renderer->CreateVertexBuffer(mesh_data->GetAllVertexSize()).release();
    auto index_buffer = renderer->CreateIndexBuffer(mesh_data->GetAllVertexIndexSize()).release();

    auto mesh = std::make_shared<Mesh>();
    mesh->SetVertexBuffer(vertex_buffer, mesh_data->GetVertexCount());
    mesh->SetIndexBuffer(index_buffer, mesh_data->GetVertexIndexCount());

    vertex_buffer->Map(mesh_data->GetOneVertexSize(), mesh_data->GetVertexData());
    index_buffer->Map(DXGI_FORMAT_R32_UINT, mesh_data->GetVertexIndexData());

    return mesh;
  }

  /**
   *  @brief  �O�p�`(1�|���S��)�̃��b�V�����쐬����
   *  @param  renderer:�����_���[
   *  @param  positions:���_���W�̔z��
   *  @return ���b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IMesh> IMesh::CreateForTriangle(IRenderer* const renderer, DirectX::XMFLOAT3 positions[3])
  {
    Vertex vertices[] = {
      { {},{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
      { {},{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
      { {},{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } }
    };
    std::uint32_t indices[] = { 0, 1, 2 };

    vertices[0].position = positions[0];
    vertices[1].position = positions[1];
    vertices[2].position = positions[2];

    auto mesh = std::make_shared<Mesh>();
    auto vertex_buffer = renderer->CreateVertexBuffer(sizeof(Vertex) * 3).release();
    auto index_buffer = renderer->CreateIndexBuffer(sizeof(std::uint32_t) * 3).release();

    mesh->SetVertexBuffer(vertex_buffer, 3);
    mesh->SetIndexBuffer(index_buffer, 3);

    vertex_buffer->Map(sizeof(Vertex), vertices);
    index_buffer->Map(DXGI_FORMAT_R32_UINT, indices);

    return mesh;
  }
};