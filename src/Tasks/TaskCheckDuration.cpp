//
// Created by alireza on 25/07/22.
//

#include "TaskCheckDuration.h"
#include "../Solver/settings.h"
#include "../Solver/Timer.h"
#include "TaskQueue.h"
namespace Scot {

TaskCheckDuration::TaskCheckDuration(EnvironmentPtr env) : TaskBase(env) {

}
void TaskCheckDuration::initialize() {

}
void TaskCheckDuration::execute() {
  auto max_time = env_->Settings->getDblSetting("max_time");
  if (env_->Timer->elapsed() >= max_time ) {
    MPI_Abort(MPI_COMM_WORLD, -1); //todo: for benchmark
    env_->TaskQueue->clearTasks();
    throw std::out_of_range("maximum time limit reached.\n");
  }

}
}