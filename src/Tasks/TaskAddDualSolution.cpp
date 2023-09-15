//
// Created by alireza on 12/06/22.
//

#include "TaskAddDualSolution.h"
#include "../MIPSolver/IMIPSolver.h"
#include "../Solver/Results.h"

namespace Scot {

TaskAddDualSolution::TaskAddDualSolution(EnvironmentPtr env) : TaskBase(env) {}

void TaskAddDualSolution::initialize() {}
void TaskAddDualSolution::execute() {

  dual_solution_.binary_vector = env_->mip_solver_->getBinarySolution();
  dual_solution_.real_vector = env_->mip_solver_->getContinuesSolution();
  dual_solution_.objective_value = env_->mip_solver_->getObjectiveValue();

  //todo: performs checks after before to the results

  env_->results_->addDualSolution(dual_solution_);
  env_->results_->iterations_.back()->number_first_order_oa_cuts_ += env_->model_->getNumberOfNodes();

}
}