//
// Created by alireza on 21/06/22.
//

#include "TaskBcastDualSolution.h"
#include "../Solver/Results.h"
#include "../Solver/MessagePassingInterface.h"
namespace Scot {

TaskBcastDualSolution::TaskBcastDualSolution(EnvironmentPtr env) : TaskBase(env) {
}
void TaskBcastDualSolution::initialize() {

}
void TaskBcastDualSolution::execute() {

  DualSolution dual_solution;

  if (env_->Model->getRank() == 0) {
    dual_solution = env_->Results->getCurrentDualSolution();
  }

  dual_solution = env_->Mpi->bcast(dual_solution);
  if (env_->Model->getRank() != 0) {
    env_->Results->addDualSolution(dual_solution);
  }
}
}