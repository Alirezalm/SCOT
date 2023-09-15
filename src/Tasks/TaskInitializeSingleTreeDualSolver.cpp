//
// Created by alireza on 11/07/22.
//

#include "TaskInitializeSingleTreeDualSolver.h"
#include "../MIPSolver/MipSolverGurobiSingleTree.h"
#include "../Solver/Logger.h"

namespace Scot {

TaskInitializeSingleTreeDualSolver::TaskInitializeSingleTreeDualSolver(EnvironmentPtr env) : TaskBase(env) {
  env_ = env_;
  env_->MipSolver = std::make_shared<MipSolverGurobiSingleTree>(env_);

  env_->Logger->logDebug("gurobi selected as a single tree MIP solver.", env_->Model->getRank());
}
void TaskInitializeSingleTreeDualSolver::initialize() {

}
void TaskInitializeSingleTreeDualSolver::execute() {

}
}