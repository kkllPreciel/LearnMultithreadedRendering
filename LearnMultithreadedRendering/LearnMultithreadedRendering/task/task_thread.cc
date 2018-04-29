/**
 *  @file     task_thread.cc
 *  @brief    タスクスレッド(ワーカースレッド)に関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

 // include
#include "task/task_thread.h"
#include <thread>
#include <atomic>

namespace App
{
  namespace
  {
    /**
     *  @brief  タスクスレッド用クラス
     */
    class TaskThread final : public ITaskThread
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      TaskThread() : thread_(nullptr), queue_(), terminate_(false)
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~TaskThread() override
      {
        Destroy();
      }

      /**
       *  @brief  作成する
       *  @param  queue:タスクキュー
       */
      void Create(std::shared_ptr<ITaskQueue> queue)
      {
        queue_ = queue;
        thread_ = std::make_unique<std::thread>(&TaskThread::Run, this);
      }
  
      /**
       *  @brief  終了する
       */
      void Destroy() override
      {
        if (thread_ && thread_->joinable())
        {
          // 終了フラグを立てる
          terminate_ = true;

          // スレッドの終了待ちを行う
          thread_->join();

          thread_.reset();
        }
      }

    private:
      /**
       *  @brief  タスクスレッドのメイン関数
       */
      void Run()
      {
        while (terminate_ == false)
        {
          std::shared_ptr<ITask> task = queue_->Pop();

          // TODO:条件変数を使って待機した方が良いか調べる(CPUが無駄に回るので)
          if (task == nullptr)
          {
            continue;
          }

          // タスクを実行する
          task->Execute();
        }
      }

    private:
      std::unique_ptr<std::thread> thread_; ///< スレッド
      std::shared_ptr<ITaskQueue> queue_;   ///< タスクキュー
      std::atomic<bool> terminate_;         ///< 終了フラグ
    };
  };

  /**
   *  @brief  タスクスレッドを作成する
   *  @param  queue:タスクのキュー
   *  @return タスクスレッドインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<ITaskThread> ITaskThread::Create(std::shared_ptr<ITaskQueue> queue)
  {
    auto task_thread = std::make_shared<TaskThread>();
    
    task_thread->Create(queue);

    return task_thread;
  }
};