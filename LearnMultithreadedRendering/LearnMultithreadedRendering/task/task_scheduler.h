/**
 *  @file     task_scheduler.h
 *  @brief    タスクスケジューラに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/16
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include "task/task_group.h"

namespace App
{
  /**
   *  @brief  タスクスケジューラ用インターフェイス
   */
  class ITaskScheduler
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    ITaskScheduler() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~ITaskScheduler() = default;

    /**
     *  @brief  タスクグループを登録する
     *  @param  task_group:登録するタスクグループ
     */
    virtual void Register(std::shared_ptr<ITaskGroup> task_group) = 0;

    /**
     *  @brief  終了する
     */
    virtual void Destroy() = 0;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    ITaskScheduler(const ITaskScheduler& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    ITaskScheduler& operator = (const ITaskScheduler& other) = delete;

    /**
     *  @brief  タスクスケジューラを作成する
     *  @param  num_threads:使用するスレッド数
     *  @return タスクスケジューラインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<ITaskScheduler> Create(const std::uint32_t num_threads);
  };
};