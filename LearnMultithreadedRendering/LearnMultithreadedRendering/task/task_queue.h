/**
 *  @file     task_queue.h
 *  @brief    タスクキューに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/18
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include "task/task.h"
#include "task/task_group.h"

namespace App
{
  /**
   *  @brief  タスクキュー用インターフェイス
   */
  class ITaskQueue
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    ITaskQueue() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~ITaskQueue() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    ITaskQueue(const ITaskQueue& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    ITaskQueue& operator = (const ITaskQueue& other) = delete;

    /**
     *  @brief  タスクグループを追加する
     *  @param  task_group:追加するタスクグループ
     */
    virtual void Push(std::shared_ptr<ITaskGroup> task_group) = 0;

    /**
     *  @brief  タスクを取得する
     *  @return タスクへのシェアードポインタ
     */
    virtual std::shared_ptr<ITask> Pop() = 0;

    /**
     *  @brief  キュー内の全タスクグループが終了したか?
     *  @return 終了フラグ
     */
    virtual bool Finished() = 0;

    /**
     *  @brief  タスクキューを作成する
     *  @return タスクキューインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<ITaskQueue> Create();
  };
};