//
// Created by alireza on 11/07/22.
//

#include "TaskInitializeSingleTreeDualSolver.h"
#include "../MIPSolver/MipSolverGurobiSingleTree.h"
#include "../Solver/Logger.h"

namespace Scot {

TaskInitializeSingleTreeDualSolver::TaskInitializeSingleTreeDualSolver(EnvironmentPtr env) : TaskBase(env) {
  env_ = env_;
  env_->mip_solver_ = std::make_shared<MipSolverGurobiSingleTree>(env_);

  env_->logger_->logDebug("gurobi selected as a single tree MIP solver.", env_->model_->getRank());
}
void TaskInitializeSingleTreeDualSolver::initialize() {

}
void TaskInitializeSingleTreeDualSolver::execute() {

}
}