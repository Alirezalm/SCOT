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

  dual_solution_.binary_vector = env_->MipSolver->getBinarySolution();
  dual_solution_.real_vector = env_->MipSolver->getContinuesSolution();
  dual_solution_.objective_value = env_->MipSolver->getObjectiveValue();

  //todo: performs checks after before to the results

  env_->Results->addDualSolution(dual_solution_);
  env_->Results->iterations_.back()->number_first_order_oa_cuts_ += env_->Model->getNumberOfNodes();

}
}