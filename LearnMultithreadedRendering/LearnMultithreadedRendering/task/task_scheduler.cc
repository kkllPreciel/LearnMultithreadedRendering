/**
 *  @file     task_scheduler.cc
 *  @brief    タスクスケジューラに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/16
 *  @version  1.0
 */

 // include
#include "task/task_scheduler.h"
#include <vector>
#include "task/task_queue.h"
#include "task/task_thread.h"

namespace App
{
  namespace
  {
    class TaskScheduler final : public ITaskScheduler
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      TaskScheduler() : queue_(nullptr), thread_list_({})
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~TaskScheduler()
      {
        Destroy();
      }

      /**
       *  @brief  作成する
       *  @param  num_threads:使用するスレッド数
       */
      void Create(const std::uint32_t num_threads)
      {
        queue_ = ITaskQueue::Create();

        for (auto i = 0; i < num_threads; ++i)
        {
          thread_list_.emplace_back(ITaskThread::Create(queue_));
        }
      }

      /**
       *  @brief  タスクグループを登録する
       *  @param  task_group:登録するタスクグループ
       */
      void Register(std::shared_ptr<ITaskGroup> task_group) override
      {
        queue_->Push(task_group);
      }
      
      /**
       *  @brief  終了する
       */
      void Destroy() override
      {
        // スレッドの削除
        for (auto& thread : thread_list_)
        {
          thread->Destroy();
        }
      }

    private:
      std::shared_ptr<ITaskQueue> queue_;                     ///< 実行するタスクのキュー
      std::vector<std::shared_ptr<ITaskThread>> thread_list_; ///< タスクスレッドのリスト
    };
  };

  /**
   *  @brief  タスクスケジューラを作成する
   *  @param  num_threads:使用するスレッド数
   *  @return タスクスケジューラインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<ITaskScheduler> ITaskScheduler::Create(const std::uint32_t num_threads)
  {
    auto task_scheduler = std::make_shared<TaskScheduler>();

    task_scheduler->Create(num_threads);

    return task_scheduler;
  }
};