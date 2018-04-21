/**
 *  @file     task.cc
 *  @brief    タスクに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

 // include
#include "task/task.h"
#include <cassert>
#include <atomic>

namespace App
{
  namespace
  {
    /**
     *  @brief  タスク用クラス
     */
    class Task final : public ITask
    {
    public:
      /**
       *  @brief  コンストラクタ
       *  @param  task:タスク関数
       *  @param  dependence_task_list:タスクが依存するタスクのリスト
       */
      Task(std::function<void(std::uint64_t)> task, std::vector<std::shared_ptr<ITask>> dependence_task_list)
        : task_(task), dependence_task_list_(dependence_task_list), finished_(false), executing_(false)
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~Task()
      {

      }

      /**
       *  @brief  タスクを実行する
       *  @param  delta_time:フレームの経過時間
       */
      void Execute(std::uint64_t delta_time) override
      {
        assert(!executing_ && !finished_);
        executing_ = true;

        for (auto task : dependence_task_list_)
        {
          if (task->Finished())
          {
            continue;
          }

          // TODO:同時に実行していると2回実行されてしまう
          task->Execute(delta_time);
        }

        task_(delta_time);
        
        executing_ = false;
        finished_ = true;
      }

      /**
       *  @brief  依存関係にあるタスクのリストを取得する
       *  @return 依存関係にあるタスクのリスト
       */
      const std::vector<std::shared_ptr<ITask>> GetDependenceTaskList() override
      {
        return dependence_task_list_;
      }

      /**
       *  @brief  タスクが終了したか?
       *  @return 終了フラグ
       */
      bool Finished() const override
      {
        return finished_;
      }

    private:
      std::function<void(std::uint64_t)> task_;   ///< タスク関数
      std::vector<std::shared_ptr<ITask>> dependence_task_list_;  ///< このタスクが依存するタスクのリスト
      bool finished_;                             ///< 終了済みフラグ




      std::atomic<bool> executing_;               ///< 実行中フラグ
    };
  };

  /**
   *  @brief  タスクを作成する
   *  @param  task_function:タスク関数
   *  @param  dependence_task_list:タスクが依存するタスクのリスト
   *  @return タスクインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<ITask> ITask::Create(std::function<void(std::uint64_t delta_time)> task_function, const std::vector<std::shared_ptr<ITask>>& dependence_task_list)
  {
    auto task = std::make_shared<Task>(task_function, dependence_task_list);
    return task;
  }
};