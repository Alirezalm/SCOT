//
// Created by alireza on 26/06/22.
//

#include "TaskCheckTerminationGap.h"
#include "../Solver/Results.h"
#include "../Solver/settings.h"
#include "TaskManager.h"

namespace scot {

TaskCheckTerminationGap::TaskCheckTerminationGap(EnvironmentPtr env) : TaskBase(env) {
  eps_abs_ = env_->settings_->getDblSetting("rel_gap");
  eps_rel_ = env_->settings_->getDblSetting("abs_gap");
  abs_gap_ = 1e10; // defaults
  rel_gap_ = 1e10;
}
void TaskCheckTerminationGap::initialize() {

}
void TaskCheckTerminationGap::execute() {
  abs_gap_ = env_->results_->getAbsoluteOptimalityGap();
  rel_gap_ = env_->results_->getRelativeOptimalityGap();

  if ((abs_gap_ <= eps_abs_) || (rel_gap_ <= eps_rel_)) {
    MPI_Barrier(MPI_COMM_WORLD);
    env_->task_manager_->clearTasks();
  }

}
}