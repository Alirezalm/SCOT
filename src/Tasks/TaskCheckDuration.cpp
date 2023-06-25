//
// Created by alireza on 25/07/22.
//

#include "TaskCheckDuration.h"
#include "../Solver/settings.h"
#include "../Solver/Timer.h"
#include "TaskQueue.h"
namespace scot {

TaskCheckDuration::TaskCheckDuration(EnvironmentPtr env) : TaskBase(env) {

}
void TaskCheckDuration::initialize() {

}
void TaskCheckDuration::execute() {
  auto max_time = env_->settings_->getDblSetting("max_time");
  if (env_->timer_->elapsed() >= max_time ) {
    MPI_Abort(MPI_COMM_WORLD, -1); //todo: for benchmark
    env_->task_queue_ptr_->clearTasks();
    throw std::out_of_range("maximum time limit reached.\n");
  }

}
}