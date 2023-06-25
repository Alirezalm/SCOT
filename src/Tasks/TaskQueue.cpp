//
// Created by alireza on 03/06/22.
//

#include "TaskQueue.h"
#include "../Solver/Logger.h"
namespace scot {

TaskQueue::TaskQueue(EnvironmentPtr env) {
  env_ = env;
}

void TaskQueue::addTask(TaskBasePtr task, std::string task_id) {
  env_->logger_
	  ->logDebug(fmt::format("adding task {} to task queue", task_id), env_->model_->getRank());

  current_task_ = std::make_pair(task, task_id);

  tasks_.push_back(current_task_);
}

void TaskQueue::clearTasks() {
  tasks_.clear();
}
PairTaskDescription TaskQueue::getTask(std::string task_id) {
  for (auto &task : tasks_) {
	if (task.second==task_id) {
	  return task;
	}
	//todo: write exception if task is not found
  }
}
bool TaskQueue::isTaskQueueEmpty() {
  return tasks_.empty();
}
}