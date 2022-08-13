//
// Created by alireza on 08/06/22.
//

#include "TaskInitializeMultipleTreeDualSolver.h"
#include "../Solver/Logger.h"
#include "../MIPSolver/IMIPSolver.h"
#include "../MIPSolver/MipSolverGurobiMultipleTree.h"
namespace scot {

void TaskInitializeMultipleTreeDualSolver::initialize() {}

void TaskInitializeMultipleTreeDualSolver::execute() {}

TaskInitializeMultipleTreeDualSolver::TaskInitializeMultipleTreeDualSolver(EnvironmentPtr env) : TaskBase(env) {
  //todo: check mip solver gurobi, clpex or cbc based on settings. for now just gurobi

  env_->mip_solver_ = std::make_shared<MipSolverGurobiMultipleTree>(env_);

  env_->logger_->logDebug("gurobi selected as MIP solver.", env_->model_->getRank());
}
}