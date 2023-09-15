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
  for (int i = 0; i < env_->model_->getNumberOfNodes(); ++i) {
    quad_oa = env_->mip_solver_->getTotalQuadraticOuterApproximations().at(i);

    env_->mip_solver_->addQuadraticOuterApproximation(quad_oa, i);
  }
}
}