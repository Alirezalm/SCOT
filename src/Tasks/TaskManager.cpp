//
// Created by alireza on 03/06/22.
//

#include "TaskManager.h"
namespace scot {

TaskManager::TaskManager(EnvironmentPtr env) {
  env_ = env;
}

void TaskManager::addTask(TaskBasePtr task, std::string task_id) {
  current_task_ = std::make_pair(task, task_id);
  tasks_.push_back(current_task_);
}

void TaskManager::clearTasks() {
  tasks_.clear();
}
PairTaskDescription TaskManager::getTask(std::string task_id) {
  for (auto &task : tasks_) {
	if (task.second == task_id) {
	  return task;
	}
	//todo: write exception if task is not found
  }
}
bool TaskManager::isTaskQueueEmpty() {
  return tasks_.empty();
}
}