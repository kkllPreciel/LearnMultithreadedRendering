/**
 *  @file     task_scheduler.cc
 *  @brief    タスクスケジューラに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/16
 *  @version  1.0
 */

 // include
#include "task_scheduler.h"
#include <deque>

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
      TaskScheduler() : task_deque_()
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
       *  @brief  タスクを登録する
       *  @param  task:タスク関数
       */
      void Register(std::function<void(std::uint64_t delta_time)> task) override
      {
        task_deque_.emplace_back(task);
      }
      
      /**
       *  @brief  タスクを実行する
       *  @param  delta_time:フレームの経過時間
       */
      void Execute(std::uint64_t delta_time) override
      {
        for (decltype(auto) task : task_deque_)
        {
          task(delta_time);
        }
      }
      
      /**
       *  @brief  終了する
       */
      void Destroy() override
      {
        task_deque_.clear();
      }

    private:
      std::deque<std::function<void(std::uint64_t)>> task_deque_; ///< 実行するタスクのdeque
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
    return task_scheduler;
  }
};