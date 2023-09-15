//
// Created by alireza on 01/07/22.
//

#include "TaskAddQuadraticOuterApproximation.h"
#include "../MIPSolver/IMIPSolver.h"
#include "../Solver/Results.h"
namespace Scot {

TaskAddQuadraticOuterApproximation::TaskAddQuadraticOuterApproximation(EnvironmentPtr env) : TaskBase(env) {

}
void TaskAddQuadraticOuterApproximation::initialize() {

}
void TaskAddQuadraticOuterApproximation::execute() {
QuadraticOuterApproximation quad_oa;
  for (int i = 0; i < env_->Model->getNumberOfNodes(); ++i) {
    quad_oa = env_->MipSolver->getTotalQuadraticOuterApproximations().at(i);

    env_->MipSolver->addQuadraticOuterApproximation(quad_oa, i);
  }
}
}