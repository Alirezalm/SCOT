//
// Created by alireza on 26/06/22.
//

#include "TaskComputeObjectiveGap.h"
#include "../Solver/Results.h"
#include "exception"
namespace Scot {

TaskComputeObjectiveGap::TaskComputeObjectiveGap(EnvironmentPtr env) : TaskBase(env) {

}
void TaskComputeObjectiveGap::initialize() {

}
void TaskComputeObjectiveGap::execute() {
  incumbent_ = env_->results_->getBestIncumbentBound();
  dual_ = env_->results_->getBestDualBound();

  //todo: write exception and throw it dual > incumbent -> error

  abs_gap_ = incumbent_ - dual_;
  rel_gap_ = abs_gap_/ (std::abs(incumbent_) + 1e-10);

  env_->results_->setAbsoluteOptimalityGap(abs_gap_);
  env_->results_->setRelativeOptimalityGap(rel_gap_);

}
}