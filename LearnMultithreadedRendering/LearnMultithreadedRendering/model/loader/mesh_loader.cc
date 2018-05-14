/**
 *  @file     mesh_loader.cc
 *  @brief    ���b�V�����[�_�[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/05/10
 *  @version  1.0
 */

 // include
#include "mesh_loader.h"
#include <vector>
#include <fstream>
#include <cassert>
#include <DirectXMath.h>

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
     *  @brief  ���b�V���f�[�^�p�N���X
     */
    class MeshData final : public IMeshData
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      MeshData() : vertices_({}), indices_({})
      {

      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~MeshData()
      {
        vertices_.clear();
        indices_.clear();
      }

      /**
       *  @brief  1���_�̃T�C�Y(�o�C�g��)���擾����
       *  @return 1���_�̃T�C�Y
       */
      std::uint32_t GetOneVertexSize() override
      {
        // TODO:���I�ɕύX�ł���悤�ɂ���
        return static_cast<std::uint32_t>(sizeof(Vertex));
      }
      
      /**
       *  @brief  �S���_���v�̃T�C�Y(�o�C�g��)���擾����
       *  @return �S���_�̃T�C�Y
       */
      std::uint32_t GetAllVertexSize() override
      {
        assert(sizeof(Vertex) * vertices_.size() < UINT32_MAX);

        // TODO:���I�ɕύX�ł���悤�ɂ���
        return static_cast<std::uint32_t>(sizeof(Vertex) * vertices_.size());
      }
      
      /**
       *  @brief  1���_�C���f�b�N�X�̃T�C�Y(�o�C�g��)���擾����
       *  @return 1���_�C���f�b�N�X�̃T�C�Y
       */
      std::uint32_t GetOneVertexIndexSize() override
      {
        // TODO:���I�ɕύX�ł���悤�ɂ���
        return static_cast<std::uint32_t>(sizeof(std::uint32_t));
      }
      
      /**
       *  @brief  �S���_�C���f�b�N�X���v�̃T�C�Y(�o�C�g��)���擾����
       *  @return �S���_�C���f�b�N�X���v�̃T�C�Y
       */
      std::uint32_t GetAllVertexIndexSize() override
      {
        assert(sizeof(std::uint32_t) * indices_.size() < UINT32_MAX);

        // TODO:���I�ɕύX�ł���悤�ɂ���
        return static_cast<std::uint32_t>(sizeof(std::uint32_t) * indices_.size());
      }
      
      /**
       *  @brief  ���_�C���f�b�N�X�����擾����
       *  @return ���_�C���f�b�N�X��
       */
      std::uint32_t GetVertexIndexCount() override
      {
        return static_cast<std::uint32_t>(indices_.size());
      }

      /**
       *  @brief  ���_�f�[�^�z��̐擪�|�C���^���擾����
       *  @return ���_�f�[�^�z��̐擪�|�C���^
       */
      void* GetVertexData() override
      {
        // TODO:�^���uvoid*�v����template�ɕύX����
        return static_cast<void*>(vertices_.data());
      }
      
      /**
       *  @brief  ���_�C���f�b�N�X�f�[�^�z��̐擪�|�C���^���擾����
       *  @return ���_�C���f�b�N�X�f�[�^�z��̐擪�|�C���^
       */
      void* GetVertexIndexData() override
      {
        // TODO:�^���uvoid*�v����template�ɕύX����
        return static_cast<void*>(indices_.data());
      }

    private:
      // TODO:���_�f�[�^�^�C�v(���W�AUV���W�A�@��)
      // TODO:���_�C���f�b�N�X�^�C�v
      std::vector<Vertex> vertices_;        ///< ���_�̔z��
      std::vector<std::uint32_t> indices_;  ///< ���_�C���f�b�N�X�̔z��
    };

    /**
     *  @brief  ���b�V���ǂݍ��ݗp�N���X
     */
    class MeshLoader final : public IMeshLoader
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      MeshLoader()
      {

      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~MeshLoader() override
      {
        Destroy();
      }

      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {

      }

      /**
       *  @brief  OBJ�t�H�[�}�b�g�t�@�C�����烁�b�V���f�[�^��ǂݍ���
       *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
       *  @return ���b�V���f�[�^�C���^�[�t�F�C�X�ւ̃|�C���^
       */
      std::shared_ptr<IMeshData> LoadFromObj(const std::string& file_path) override
      {
        // TODO:�g���q�`�F�b�N

        std::ifstream stream;
        stream.open(file_path, std::ifstream::in | std::ifstream::binary);
        if (false == stream.operator bool())
        {
          throw std::exception("�t�@�C���̃I�[�v���Ɏ��s���܂����B");
        }

        // �ǂݍ��ݎ��s
        std::istreambuf_iterator<char> it(stream);
        std::istreambuf_iterator<char> last;
        std::string data(it, last);
        stream.close();

        auto mesh_data = std::make_shared<MeshData>();

//        std::unique_ptr<Model> model = nullptr;
//
//
//        // C4996�x���𖳌��ɂ���
//#if defined(_MSC_VER) && _MSC_VER >= 1400
//#pragma warning(push)
//#pragma warning(disable:4996)
//#endif
//        // �s���ɕ�������
//        std::vector<std::string> list;
//        boost::split(list, data, boost::is_any_of("\r\n"));
//#if defined(_MSC_VER) && _MSC_VER >= 1400
//#pragma warning(pop)
//#endif
//
//        model = std::make_unique<Model>();
//
//        // ���f���f�[�^�ǂݍ���(�s�P�ʂŏ������s��)
//        for (auto line : list)
//        {
//          std::vector<std::string> parts;
//          boost::split(parts, line, boost::is_space());
//
//          // �����o���Ȃ�����(��@�ȃf�[�^)
//          if (parts.size() <= 1)
//          {
//            continue;
//          }
//
//          // �L�[���[�h���擾���Ή����鏈�����s��
//          switch (GetID(parts.at(0)))
//          {
//            // �R�����g
//          case ObjKeywords::kComment:
//            continue;
//            break;
//            // �}�e���A���t�@�C����
//          case ObjKeywords::kMaterialFileName:
//            continue;
//            break;
//            // �O���[�v
//          case ObjKeywords::kGroup:
//            continue;
//            break;
//            // ���݂̃O���[�v�f�[�^���g�p����}�e���A����
//          case ObjKeywords::kUseMaterial:
//            break;
//            // ���_���W
//          case ObjKeywords::kControlPoint:
//            // TODO:�v�f����4�łȂ��ꍇ�͈�@�f�[�^�Ƃ��Ė߂�
//            model->AddControlPoint(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
//            break;
//            // �e�N�X�`�����W
//          case ObjKeywords::kVertexTexture:
//            // TODO:�v�f����3�łȂ��ꍇ�͈�@�f�[�^�Ƃ��Ė߂�
//            model->AddTextureCoord(DirectX::XMFLOAT2(std::stof(parts[1]), std::stof(parts[2])));
//            break;
//            // ���_�@���x�N�g��
//          case ObjKeywords::kVertexNormal:
//            // TODO:�v�f����4�łȂ��ꍇ�͈�@�f�[�^�Ƃ��Ė߂�
//            model->AddNormal(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
//            break;
//            // �|���S���f�[�^
//          case ObjKeywords::kPolygon:
//          {
//            std::vector<uint32_t> point_indices;
//            std::vector<uint32_t> normal_indices;
//            std::vector<uint32_t> texture_coord_indices;
//            for (decltype(parts.size()) i = 1; i < parts.size(); ++i)
//            {
//              std::vector<std::string> indices;
//              boost::split(indices, parts[i], boost::is_any_of("/"));
//              for (auto index : indices | boost::adaptors::indexed())
//              {
//                if (false == index.value().empty())
//                {
//                  // ���W�E�e�N�X�`�����W�E�@���x�N�g����0�Ԗڂ���Ȃ̂ŁA-1���s��
//                  const auto value = std::stoi(index.value()) - 1;
//                  switch (index.index())
//                  {
//                  case 0:
//
//                    point_indices.emplace_back(value);
//                    break;
//                  case 1:
//                    texture_coord_indices.emplace_back(value);
//                    break;
//                  case 2:
//                    normal_indices.emplace_back(value);
//                    break;
//                  }
//                }
//              }
//            }
//            model->AddPointIndex(point_indices);
//            model->AddTextureCoordIndex(texture_coord_indices);
//            model->AddNormalIndex(normal_indices);
//            model->AddPolygon();
//          }
//          break;
//          // �s���L�[���[�h
//          default:
//            continue;
//            break;
//          }
//        }
//
//        return model;

        return mesh_data;
      }

    private:
    };
  };

  /**
   *  @brief  ���b�V�����[�_�[���쐬����
   *  @return ���b�V�����[�_�[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IMeshLoader> IMeshLoader::Create()
  {
    auto mesh_loader = std::make_shared<MeshLoader>();

    return mesh_loader;
  }
};