/**
 *  @file     double_command_list.h
 *  @brief    ダブルコマンドリストに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/15
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <Sein/Direct3D12/command_list.h>
#include <Sein/Direct3D12/direct3d12_device.h>

namespace App
{
  /**
   *  @brief  ダブルバッファリングを行うコマンドリスト用インターフェイス
   */
  class IDoubleCommandList
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IDoubleCommandList() = default;
  
    /**
     *  @brief  デストラクタ
     */
    virtual ~IDoubleCommandList() = default;

    /**
     *  @brief  コマンドリストの交換を行う
     */
    virtual void Swap() = 0;

    /**
     *  @brief  現在フロントのコマンドリストを取得する
     *  @return コマンドリストへの参照
     */
    virtual const Sein::Direct3D12::ICommandList& GetFrontCommandList() = 0;
    
    /**
     *  @brief  現在バックのコマンドリストを取得する
     *  @return コマンドリストへの参照
     */
    virtual Sein::Direct3D12::ICommandList& GetBackCommandList() = 0;
  
    /**
     *  @brief  終了処理を行う
     */
    virtual void Destroy() = 0;
  
    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IDoubleCommandList(const IDoubleCommandList& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    IDoubleCommandList& operator = (const IDoubleCommandList& other) = delete;

    /**
     *  @brief  ダブルコマンドリストを作成する
     *  @param  device:デバイス
     *  @param  command_list_type:コマンドリストのタイプ
     *  @return ダブルコマンドリストインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IDoubleCommandList> Create(std::shared_ptr<Sein::Direct3D12::IDevice> device, const D3D12_COMMAND_LIST_TYPE& command_list_type);
  };
};