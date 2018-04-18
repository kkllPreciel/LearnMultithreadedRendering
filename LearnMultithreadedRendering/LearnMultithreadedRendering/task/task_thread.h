/**
 *  @file     task_thread.h
 *  @brief    タスクスレッド(ワーカースレッド)に関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

#pragma once

// include
#include "task/task_queue.h"

namespace App
{
  /**
   *  @brief  タスクスレッド用インターフェイス
   */
  class ITaskThread
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    ITaskThread() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~ITaskThread() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    ITaskThread(const ITaskThread& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    ITaskThread& operator = (const ITaskThread& other) = delete;

    /**
     *  @brief  実行する
     *  @param  delta_time:フレームの経過時間
     */
    virtual void Execute(std::uint64_t delta_time) = 0;

    /**
     *  @brief  終了する
     */
    virtual void Destroy() = 0;

    /**
     *  @brief  タスクスレッドを作成する
     *  @param  queue:タスクのキュー
     *  @return タスクスレッドインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<ITaskThread> Create(std::shared_ptr<ITaskQueue> queue);
  };
};