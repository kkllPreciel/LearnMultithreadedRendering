/**
 *  @file     task_queue.h
 *  @brief    タスクキューに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/18
 *  @version  1.0
 */

#pragma once

// include
#include "task/task.h"

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
     *  @brief  実行の準備を行う
     */
    virtual void MakeReady() = 0;

    /**
     *  @brief  タスクを追加する
     *  @param  task:追加するタスク
     */
    virtual void Push(ITask* task) = 0;

    /**
     *  @brief  タスクを取得する
     *  @return タスクへのポインタ
     */
    virtual ITask* Pop() = 0;

    /**
     *  @brief  キューをクリアする
     */
    virtual void Clear() = 0;

    /**
     *  @brief  タスクキューを作成する
     *  @return タスクキューインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<ITaskQueue> Create();
  };
};