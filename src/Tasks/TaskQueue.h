//
// Created by alireza on 03/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKMANAGER_H_
#define DISCOT_SRC_TASKS_TASKMANAGER_H_
#include "../Solver/Environment.h"
#include "../Tasks/TaskBase.h"
namespace scot {
class TaskQueue {

 public:
  explicit TaskQueue(EnvironmentPtr env);
  ~TaskQueue() = default;

  void addTask(TaskBasePtr task, std::string task_id);
  bool isTaskQueueEmpty();
  void clearTasks();
  PairTaskDescription getTask(std::string task_id);
  std::vector<PairTaskDescription> tasks_;

 private:
  PairTaskDescription current_task_;
  EnvironmentPtr env_;


};

}

#endif //DISCOT_SRC_TASKS_TASKMANAGER_H_
