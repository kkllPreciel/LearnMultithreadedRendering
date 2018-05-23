/**
 *  @file     double_command_list.cc
 *  @brief    ダブルコマンドリストに関する処理を行うプログラムソース
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
       *  @brief  コンストラクタ
       */
      DoubleCommandList() : front_(nullptr), back_(nullptr)
      {

      }
    
      /**
       *  @brief  デストラクタ
       */
      ~DoubleCommandList() override
      {
        Destroy();
      }

      /**
       *  @brief  コマンドリストを作成する
       *  @param  device:デバイス
       *  @param  command_list_type:コマンドリストのタイプ
       */
      void Create(std::shared_ptr<Sein::Direct3D12::IDevice> device, const D3D12_COMMAND_LIST_TYPE& command_list_type)
      {
        front_ = device->CreateCommandList(command_list_type);
        back_ = device->CreateCommandList(command_list_type);
      }
  
      /**
       *  @brief  コマンドリストの交換を行う
       */
      void Swap() override
      {
        front_.swap(back_);
      }
  
      /**
       *  @brief  現在フロントのコマンドリストを取得する
       *  @return コマンドリストへの参照
       */
      const Sein::Direct3D12::ICommandList& GetFrontCommandList() override
      {
        return *front_;
      }
      
      /**
       *  @brief  現在バックのコマンドリストを取得する
       *  @return コマンドリストへの参照
       */
      Sein::Direct3D12::ICommandList& GetBackCommandList() override
      {
        return *back_;
      }
    
      /**
       *  @brief  終了処理を行う
       */
      void Destroy() override
      {
        front_.reset();
        back_.reset();
      }

    private:
      std::shared_ptr<Sein::Direct3D12::ICommandList> front_; ///< フロントのコマンドリスト
      std::shared_ptr<Sein::Direct3D12::ICommandList> back_;  ///< バックのコマンドリスト
    };
  };

  /**
   *  @brief  ダブルコマンドリストを作成する
   *  @param  device:デバイス
   *  @param  command_list_type:コマンドリストのタイプ
   *  @return ダブルコマンドリストインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<IDoubleCommandList> IDoubleCommandList::Create(std::shared_ptr<Sein::Direct3D12::IDevice> device, const D3D12_COMMAND_LIST_TYPE& command_list_type)
  {
    auto double_command_list = std::make_shared<DoubleCommandList>();

    double_command_list->Create(device, command_list_type);

    return double_command_list;
  }
};