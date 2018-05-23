/**
 *  @file     double_command_list.cc
 *  @brief    �_�u���R�}���h���X�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/15
 *  @version  1.0
 */

 // include
#include "renderer/double_command_list.h"

namespace App
{
  namespace
  {
    class DoubleCommandList final : public IDoubleCommandList
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      DoubleCommandList() : front_(nullptr), back_(nullptr)
      {

      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~DoubleCommandList() override
      {
        Destroy();
      }

      /**
       *  @brief  �R�}���h���X�g���쐬����
       *  @param  device:�f�o�C�X
       *  @param  command_list_type:�R�}���h���X�g�̃^�C�v
       */
      void Create(std::shared_ptr<Sein::Direct3D12::IDevice> device, const D3D12_COMMAND_LIST_TYPE& command_list_type)
      {
        front_ = device->CreateCommandList(command_list_type);
        back_ = device->CreateCommandList(command_list_type);
      }
  
      /**
       *  @brief  �R�}���h���X�g�̌������s��
       */
      void Swap() override
      {
        front_.swap(back_);
      }
  
      /**
       *  @brief  ���݃t�����g�̃R�}���h���X�g���擾����
       *  @return �R�}���h���X�g�ւ̎Q��
       */
      const Sein::Direct3D12::ICommandList& GetFrontCommandList() override
      {
        return *front_;
      }
      
      /**
       *  @brief  ���݃o�b�N�̃R�}���h���X�g���擾����
       *  @return �R�}���h���X�g�ւ̎Q��
       */
      Sein::Direct3D12::ICommandList& GetBackCommandList() override
      {
        return *back_;
      }
    
      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {
        front_.reset();
        back_.reset();
      }

    private:
      std::shared_ptr<Sein::Direct3D12::ICommandList> front_; ///< �t�����g�̃R�}���h���X�g
      std::shared_ptr<Sein::Direct3D12::ICommandList> back_;  ///< �o�b�N�̃R�}���h���X�g
    };
  };

  /**
   *  @brief  �_�u���R�}���h���X�g���쐬����
   *  @param  device:�f�o�C�X
   *  @param  command_list_type:�R�}���h���X�g�̃^�C�v
   *  @return �_�u���R�}���h���X�g�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IDoubleCommandList> IDoubleCommandList::Create(std::shared_ptr<Sein::Direct3D12::IDevice> device, const D3D12_COMMAND_LIST_TYPE& command_list_type)
  {
    auto double_command_list = std::make_shared<DoubleCommandList>();

    double_command_list->Create(device, command_list_type);

    return double_command_list;
  }
};