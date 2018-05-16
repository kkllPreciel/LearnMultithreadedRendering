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
#include <map>
#include <unordered_map>

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <DirectXMath.h>

namespace MeshLoader
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

    enum ObjKeywords {
      kIllegal = 0,
      kComment,
      kMaterialFileName,
      kGroup,
      kUseMaterial,
      kControlPoint,
      kVertexTexture,
      kVertexNormal,
      kPolygon,
    };

    ObjKeywords GetID(std::string keyword)
    {
      std::map<std::string, ObjKeywords> map =
      {
        { "#", ObjKeywords::kComment },
        { "mtllib", ObjKeywords::kMaterialFileName },
        { "g", ObjKeywords::kGroup },
        { "usemtl", ObjKeywords::kUseMaterial },
        { "v", ObjKeywords::kControlPoint },
        { "vt", ObjKeywords::kVertexTexture },
        { "vn", ObjKeywords::kVertexNormal },
        { "f", ObjKeywords::kPolygon },
      };

      if (map.count(keyword) == 0)
      {
        return ObjKeywords::kIllegal;
      }

      return map.at(keyword);
    }

    /**
     *  @brief  ���b�V���f�[�^�p�N���X
     */
    class MeshData final : public IMeshData
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       *  @param  vertices:���_�f�[�^�z��
       *  @param  indices:���_�C���f�b�N�X�f�[�^�z��
       */
      MeshData(std::vector<Vertex>& vertices, std::vector<std::uint32_t>& indices) : vertices_(vertices), indices_(indices)
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
       *  @brief  ���_�����擾����
       *  @return ���_��
       */
      std::uint32_t GetVertexCount() override
      {
        return static_cast<std::uint32_t>(vertices_.size());
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

        // �s���ɕ�������
        std::vector<std::string> list;
        boost::split(list, data, boost::is_any_of("\r\n"));

        // ���_�f�[�^
        std::vector<DirectX::XMFLOAT3> control_points;
        std::vector<DirectX::XMFLOAT2> texture_coords;
        std::vector<DirectX::XMFLOAT3> normals;

        // ���_�C���f�b�N�X�f�[�^
        std::vector<uint32_t> point_indices;
        std::vector<uint32_t> normal_indices;
        std::vector<uint32_t> texture_coord_indices;

        // ���b�V���f�[�^�ǂݍ���(�s�P�ʂŏ������s��)
        for (auto line : list)
        {
          std::vector<std::string> parts;
          boost::split(parts, line, boost::is_space());

          // �����o���Ȃ�����(��@�ȃf�[�^)
          if (parts.size() <= 1)
          {
            continue;
          }

          // �L�[���[�h���擾���Ή����鏈�����s��
          switch (GetID(parts.at(0)))
          {
            // �R�����g
          case ObjKeywords::kComment:
            continue;
            break;
            // �}�e���A���t�@�C����
          case ObjKeywords::kMaterialFileName:
            continue;
            break;
            // �O���[�v
          case ObjKeywords::kGroup:
            continue;
            break;
            // ���݂̃O���[�v�f�[�^���g�p����}�e���A����
          case ObjKeywords::kUseMaterial:
            break;
            // ���_���W
          case ObjKeywords::kControlPoint:
            // TODO:�v�f����4�łȂ��ꍇ�͈�@�f�[�^�Ƃ��Ė߂�
            control_points.emplace_back(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
            break;
            // �e�N�X�`�����W
          case ObjKeywords::kVertexTexture:
            // TODO:�v�f����3�łȂ��ꍇ�͈�@�f�[�^�Ƃ��Ė߂�
            texture_coords.emplace_back(DirectX::XMFLOAT2(std::stof(parts[1]), std::stof(parts[2])));
            break;
            // ���_�@���x�N�g��
          case ObjKeywords::kVertexNormal:
            // TODO:�v�f����4�łȂ��ꍇ�͈�@�f�[�^�Ƃ��Ė߂�
            normals.emplace_back(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
            break;
            // �|���S���f�[�^
          case ObjKeywords::kPolygon:
          {
            for (decltype(parts.size()) i = 1; i < parts.size(); ++i)
            {
              std::vector<std::string> indices;
              boost::split(indices, parts[i], boost::is_any_of("/"));
              for (auto index : indices | boost::adaptors::indexed())
              {
                if (false == index.value().empty())
                {
                  // ���W�E�e�N�X�`�����W�E�@���x�N�g����0�Ԗڂ���Ȃ̂ŁA-1���s��
                  const auto value = std::stoi(index.value()) - 1;
                  switch (index.index())
                  {
                  case 0:
                    point_indices.emplace_back(value);
                    break;
                  case 1:
                    texture_coord_indices.emplace_back(value);
                    break;
                  case 2:
                    normal_indices.emplace_back(value);
                    break;
                  }
                }
              }
            }
          }
          break;
          // �s���L�[���[�h
          default:
            continue;
            break;
          }
        }
        
        // ���W�EUV���W�E�@���x�N�g�����璸�_�f�[�^�𐶐�����
        std::vector<Vertex> vertices;
        std::unordered_map<std::string, uint32_t> map;
        std::vector<uint32_t> indices;

        // ���_�f�[�^���X�g�ƁA���_�f�[�^�C���f�b�N�X�ƃn�b�V���������X�g���쐬����
        for (const auto point : point_indices | boost::adaptors::indexed())
        {
          const auto index = point.index();

          // �n�b�V�����쐬
          std::string hash = std::to_string(point.value());

          if (texture_coord_indices.empty() == false)
          {
            hash += '-' + std::to_string(texture_coord_indices[index]);
          }

          if (normal_indices.empty() == false)
          {
            hash += '-' + std::to_string(normal_indices[index]);
          }

          // �n�b�V�������݂��Ȃ��ꍇ�͒��_�f�[�^��ǉ�����
          if (map.count(hash) == 0)
          {
            // ���_�f�[�^
            Vertex vertex = {};
            vertex.position = control_points.at(point.value());

            if (normal_indices.empty() == false)
            {
              vertex.normal = normals.at(normal_indices.at(index));
            }

            if (texture_coord_indices.empty() == false)
            {
              vertex.texcoord = texture_coords.at(texture_coord_indices.at(index));
            }

            vertices.emplace_back(vertex);

            // �n�b�V�����X�g���X�V
            map.insert({ hash, static_cast<uint32_t>(vertices.size() - 1) });
          }

          // �C���f�b�N�X���X�g�ɃC���f�b�N�X��ǉ�����
          indices.emplace_back(map.at(hash));
        }

        auto mesh_data = std::make_shared<MeshData>(vertices, indices);

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