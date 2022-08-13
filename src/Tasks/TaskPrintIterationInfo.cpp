//
// Created by alireza on 26/06/22.
//

#include "TaskPrintIterationInfo.h"
#include "../Solver/Report.h"
#include "../Solver/Results.h"
#include "../Solver/Timer.h"
namespace scot {

TaskPrintIterationInfo::TaskPrintIterationInfo(EnvironmentPtr env) : TaskBase(env) {

}
void TaskPrintIterationInfo::initialize() {

}
void TaskPrintIterationInfo::execute() {
//  if (env_->model_->getRank() == 0) {
  auto current_iter = env_->results_->getCurrentIteration();
  env_->report_->printIterationInfo(
      current_iter->number_first_order_oa_cuts_,
      current_iter->number_second_order_oa_cuts_,
      env_->results_->getBestIncumbentBound(),
      env_->results_->getBestDualBound(),
      env_->results_->getAbsoluteOptimalityGap(),
      env_->results_->getRelativeOptimalityGap(),
      env_->timer_->elapsed()
  );
//  }
}
}