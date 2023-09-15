//
// Created by alireza on 26/06/22.
//

#include "TaskPrintIterationInfo.h"
#include "../Solver/Report.h"
#include "../Solver/Results.h"
#include "../Solver/Timer.h"
namespace Scot {

TaskPrintIterationInfo::TaskPrintIterationInfo(EnvironmentPtr env) : TaskBase(env) {

}
void TaskPrintIterationInfo::initialize() {

}
void TaskPrintIterationInfo::execute() {
  if (env_->Model->getRank() == 0) {
  auto current_iter = env_->Results->getCurrentIteration();
  env_->Report->printIterationInfo(
      current_iter->number_first_order_oa_cuts_,
      current_iter->number_second_order_oa_cuts_,
      env_->Results->getBestIncumbentBound(),
      env_->Results->getBestDualBound(),
      env_->Results->getAbsoluteOptimalityGap(),
      env_->Results->getRelativeOptimalityGap(),
	  env_->Timer->elapsed()
  );
  }
}
}