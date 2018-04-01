/**
 *  @file     render_mesh.cc
 *  @brief    �`��p���b�V���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/01
 *  @version  1.0
 */

 // include
#include "render_mesh.h"

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
     *  @brief  �`��p���b�V���p�N���X
     */
    class RenderMesh final : public IRenderMesh
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      RenderMesh() : vertex_buffer_(nullptr), index_buffer_(nullptr), vertex_count_(0), index_count_(0)
      {

      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~RenderMesh() override
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
      void SetVertexBuffer(Sein::Direct3D12::VertexBuffer* const vertex_buffer, const std::uint32_t vertex_count)
      {
        vertex_buffer_.reset(vertex_buffer);
        vertex_count_ = vertex_count;
      }

      /**
       *  @brief  �C���f�b�N�X�o�b�t�@��ݒ肷��
       *  @param  index_buffer:�C���f�b�N�o�b�t�@
       *  @param  index_count:�C���f�b�N�X��
       */
      void SetIndexBuffer(Sein::Direct3D12::IndexBuffer* const index_buffer, const std::uint32_t index_count)
      {
        index_buffer_.reset(index_buffer);
        index_count_ = index_count;
      }
      
      /**
       *  @brief  ���_�o�b�t�@���擾����
       *  @return ���_�o�b�t�@
       */
      const Sein::Direct3D12::VertexBuffer& GetVertexBuffer() const override
      {
        return *vertex_buffer_;
      }
      
      /**
       *  @brief  �C���f�b�N�X�o�b�t�@���擾����
       *  @return �C���f�b�N�X�o�b�t�@
       */
      const Sein::Direct3D12::IndexBuffer& GetIndexBuffer() const override
      {
        return *index_buffer_;
      }
      
      /**
       *  @brief  ���_�C���f�b�N�X�̌����擾����
       *  @param  ���_�C���f�b�N�X�̌�
       */
      std::uint32_t GetIndexCount() const override
      {
        return index_count_;
      }

    private:
      std::unique_ptr<Sein::Direct3D12::VertexBuffer> vertex_buffer_; ///< ���_�o�b�t�@
      std::unique_ptr<Sein::Direct3D12::IndexBuffer> index_buffer_;   ///< �C���f�b�N�X�o�b�t�@
      std::uint32_t vertex_count_;                                    ///< ���_��
      std::uint32_t index_count_;                                     ///< �C���f�b�N�X��
    };
  };

  /**
   *  @brief  �`��p�̃��b�V�����쐬����
   *  @param  device:�f�o�C�X
   *  @param  vertices:���_�z��ւ̃|�C���^
   *  @param  vertex_count:���_��
   *  @param  indices:���_�C���f�b�N�X�z��ւ̃|�C���^
   *  @param  index_count:���_�C���f�b�N�X��
   *  @return �`��p�̃��b�V���p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IRenderMesh> IRenderMesh::Create(Sein::Direct3D12::Device* device, void* const vertices, std::uint32_t vertex_count, void* const indices, std::uint32_t index_count)
  {
    auto render_mesh = std::make_shared<RenderMesh>();
    auto vertex_buffer = new Sein::Direct3D12::VertexBuffer();
    auto index_buffer = new Sein::Direct3D12::IndexBuffer();

    render_mesh->SetVertexBuffer(vertex_buffer, vertex_count);
    render_mesh->SetIndexBuffer(index_buffer, index_count);
        
    vertex_buffer->Create(const_cast<ID3D12Device*>(&device->GetDevice()), sizeof(Vertex) * vertex_count, sizeof(Vertex), vertices);
    index_buffer->Create(const_cast<ID3D12Device*>(&device->GetDevice()), sizeof(std::uint32_t) * index_count, indices, DXGI_FORMAT_R32_UINT);
    
    return render_mesh;
  }
};