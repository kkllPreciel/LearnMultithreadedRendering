/**
 *  @file     task_scheduler.cc
 *  @brief    タスクスケジューラに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/16
 *  @version  1.0
 */

 // include
#include "task/task_scheduler.h"
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
       *  @brief  タスクを登録する
       *  @param  task:タスク
       */
      void Register(ITask* task) override
      {
        queue_->Push(task);
      }
      
      /**
       *  @brief  タスクを実行する
       *  @param  delta_time:フレームの経過時間
       */
      void Execute(std::uint64_t delta_time) override
      {
        queue_->MakeReady();

        // スレッドの起床
        for (auto& thread : thread_list_)
        {
          thread->Execute(delta_time);
        }

        // タスクの実行
        ITask* task = nullptr;
        while ((task = queue_->Pop()) != nullptr)
        {
          task->Execute(delta_time);
        }

        // TODO:別スレッドでタスクが実行中かもしれないので待機する

        queue_->Clear();
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