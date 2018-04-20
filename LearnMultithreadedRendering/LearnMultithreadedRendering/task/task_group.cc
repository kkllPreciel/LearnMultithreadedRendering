/**
 *  @file     task_group.cc
 *  @brief    タスクグループに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/20
 *  @version  1.0
 */

 // include
#include "task/task_group.h"

namespace App
{
  namespace
  {
    /**
     *  @brief  タスクグループ用クラス
     */
    class TaskGroup final : public ITaskGroup
    {
    public:
      /**
       *  @brief  コンストラクタ
       *  @param  task_list:タスクグループに属するタスクのリスト
       *  @param  dependence_task_group_list:依存先のタスクグループのリスト
       */
      TaskGroup(std::vector<std::shared_ptr<ITask>>& task_list, std::vector<std::shared_ptr<ITaskGroup>>& dependence_task_group_list)
        : task_list_(task_list), dependence_task_group_list_(dependence_task_group_list)
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~TaskGroup() override
      {

      }

      /**
       *  @brief  タスクグループに含まれるタスクの個数を取得する
       *  @return タスクの個数
       */
      const std::uint32_t GetTaskCount() const override
      {
        return task_list_.size();
      }
      
      /**
       *  @brief  指定された番号のタスクを取得する
       *  @param  index:タスクの番号
       *  @return タスクへのシェアードポインタ
       */
      std::shared_ptr<ITask> GetTask(const std::uint32_t index) const override
      {
        return task_list_[index];
      }

    private:
      std::vector<std::shared_ptr<ITask>> task_list_;                         ///< タスクグループに属するタスクのリスト
      std::vector<std::shared_ptr<ITaskGroup>>  dependence_task_group_list_;  ///< 依存先のタスクグループのリスト
    };
  };

  /**
   *  @brief  タスクグループを作成する
   *  @param  task_list:タスクのリスト
   *  @param  dependence_task_group_list:タスクグループが依存するタスクグループのリスト
   *  @return タスクグループインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<ITaskGroup> ITaskGroup::Create(std::vector<std::shared_ptr<ITask>>& task_list, std::vector<std::shared_ptr<ITaskGroup>>& dependence_task_group_list)
  {
    auto task_group = std::make_shared<TaskGroup>(task_list, dependence_task_group_list);
    return task_group;
  }
};