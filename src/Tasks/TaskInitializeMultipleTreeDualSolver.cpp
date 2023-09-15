//
// Created by alireza on 08/06/22.
//

#include "TaskInitializeMultipleTreeDualSolver.h"
#include "../Solver/Logger.h"
#include "../MIPSolver/IMIPSolver.h"
#include "../MIPSolver/MipSolverGurobiMultipleTree.h"
namespace Scot {

TaskInitializeMultipleTreeDualSolver::TaskInitializeMultipleTreeDualSolver(EnvironmentPtr env) : TaskBase(env) {
  //todo: check mip solver gurobi, clpex or cbc based on settings. for now just gurobi

  env_->MipSolver = std::make_shared<MipSolverGurobiMultipleTree>(env_);

  env_->Logger->logDebug("gurobi selected as MIP solver.", env_->Model->getRank());
}

void TaskInitializeMultipleTreeDualSolver::initialize() {}

void TaskInitializeMultipleTreeDualSolver::execute() {}
}