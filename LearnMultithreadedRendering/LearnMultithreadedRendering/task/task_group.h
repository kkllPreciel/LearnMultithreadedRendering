/**
 *  @file     task_group.h
 *  @brief    タスクグループに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/20
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include "task/task.h"

namespace App
{
  /**
   *  @brief  タスクグループ用インターフェイス
   */
  class ITaskGroup
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    ITaskGroup() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~ITaskGroup() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    ITaskGroup(const ITaskGroup& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    ITaskGroup& operator = (const ITaskGroup& other) = delete;

    /**
     *  @brief  タスクグループに含まれるタスクの個数を取得する
     *  @return タスクの個数
     */
    virtual const std::uint32_t GetTaskCount() const = 0;

    /**
     *  @brief  指定された番号のタスクを取得する
     *  @param  index:タスクの番号
     *  @return タスクへのシェアードポインタ
     */
    virtual std::shared_ptr<ITask> GetTask(const std::uint32_t index) const = 0;

    /**
     *  @brief  タスクグループを作成する
     *  @param  task_list:タスクのリスト
     *  @param  dependence_task_group_list:タスクグループが依存するタスクグループのリスト
     *  @return タスクグループインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<ITaskGroup> Create(std::vector<std::shared_ptr<ITask>>& task_list, std::vector<std::shared_ptr<ITaskGroup>>& dependence_task_group_list);
  };
};