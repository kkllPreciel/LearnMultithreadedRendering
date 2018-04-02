/**
 *  @file     mesh.cc
 *  @brief    メッシュに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/02
 *  @version  1.0
 */

 // include
#include "mesh.h"

namespace App
{
  namespace
  {
    // アラインメントを1バイトに設定
#pragma pack(push, 1)
    struct Vertex
    {
      DirectX::XMFLOAT3 position; ///< 座標
      DirectX::XMFLOAT3 normal;   ///< 法線ベクトル
      DirectX::XMFLOAT2 texcoord; ///< テクスチャUV座標
    };
#pragma pack(pop)

    /**
     *  @brief  メッシュ用クラス
     */
    class Mesh final : public IMesh
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Mesh() : vertex_buffer_(nullptr), index_buffer_(nullptr), vertex_count_(0), index_count_(0)
      {

      }
    
      /**
       *  @brief  デストラクタ
       */
      ~Mesh() override
      {
        Destroy();
      }

      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {
        vertex_buffer_.reset();
        index_buffer_.reset();
        vertex_count_ = 0;
        index_count_ = 0;
      }

      /**
       *  @brief  頂点バッファを設定する
       *  @param  vertex_buffer:頂点バッファ
       *  @param  vertex_count:頂点数
       */
      void SetVertexBuffer(Sein::Direct3D12::IVertexBuffer* const vertex_buffer, const std::uint32_t vertex_count)
      {
        vertex_buffer_.reset(vertex_buffer);
        vertex_count_ = vertex_count;
      }

      /**
       *  @brief  インデックスバッファを設定する
       *  @param  index_buffer:インデックバッファ
       *  @param  index_count:インデックス数
       */
      void SetIndexBuffer(Sein::Direct3D12::IndexBuffer* const index_buffer, const std::uint32_t index_count)
      {
        index_buffer_.reset(index_buffer);
        index_count_ = index_count;
      }
      
      /**
       *  @brief  頂点バッファを取得する
       *  @return 頂点バッファ
       */
      const Sein::Direct3D12::IVertexBuffer& GetVertexBuffer() const override
      {
        return *vertex_buffer_;
      }
      
      /**
       *  @brief  インデックスバッファを取得する
       *  @return インデックスバッファ
       */
      const Sein::Direct3D12::IndexBuffer& GetIndexBuffer() const override
      {
        return *index_buffer_;
      }
      
      /**
       *  @brief  頂点インデックスの個数を取得する
       *  @param  頂点インデックスの個数
       */
      std::uint32_t GetIndexCount() const override
      {
        return index_count_;
      }

    private:
      std::unique_ptr<Sein::Direct3D12::IVertexBuffer> vertex_buffer_;  ///< 頂点バッファ
      std::unique_ptr<Sein::Direct3D12::IndexBuffer> index_buffer_;     ///< インデックスバッファ
      std::uint32_t vertex_count_;                                      ///< 頂点数
      std::uint32_t index_count_;                                       ///< インデックス数
    };
  };

  /**
   *  @brief  メッシュを作成する
   *  @param  device:デバイス
   *  @param  vertices:頂点配列へのポインタ
   *  @param  vertex_count:頂点数
   *  @param  indices:頂点インデックス配列へのポインタ
   *  @param  index_count:頂点インデックス数
   *  @return メッシュ用インターフェイスへのシェアードポインタ
   */
  std::shared_ptr<IMesh> IMesh::Create(Sein::Direct3D12::Device* device, void* const vertices, std::uint32_t vertex_count, void* const indices, std::uint32_t index_count)
  {
    auto mesh = std::make_shared<Mesh>();
    auto vertex_buffer = Sein::Direct3D12::IVertexBuffer::Create(const_cast<ID3D12Device*>(&device->GetDevice()), sizeof(Vertex) * vertex_count).release();
    auto index_buffer = new Sein::Direct3D12::IndexBuffer();

    mesh->SetVertexBuffer(vertex_buffer, vertex_count);
    mesh->SetIndexBuffer(index_buffer, index_count);
        
    vertex_buffer->Map(sizeof(Vertex), vertices);
    index_buffer->Create(const_cast<ID3D12Device*>(&device->GetDevice()), sizeof(std::uint32_t) * index_count, indices, DXGI_FORMAT_R32_UINT);
    
    return mesh;
  }
};