//
// Created by alireza on 12/06/22.
//

#include "TaskAddLinearOuterApproximation.h"
#include "../MIPSolver/IMIPSolver.h"
#include "../Solver/Results.h"

namespace Scot {

TaskAddLinearOuterApproximation::TaskAddLinearOuterApproximation(EnvironmentPtr env) : TaskBase(env) {

}
void TaskAddLinearOuterApproximation::initialize() {

}
void TaskAddLinearOuterApproximation::execute() {

  LinearOuterApproximation lin_oa;
  for (int i = 0; i < env_->Model->getNumberOfNodes(); ++i) {
    lin_oa = env_->MipSolver->getTotalLinearOuterApproximations().at(i);

    env_->MipSolver->addLinearOuterApproximation(lin_oa, i);
  }
}
}