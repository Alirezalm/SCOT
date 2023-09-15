//
// Created by alireza on 12/07/22.
//

#include "TaskSolveSingleTreeDualProblem.h"
#include "../MIPSolver/IMIPSolver.h"
#include "../Tasks/TaskQueue.h"
namespace Scot {

TaskSolveSingleTreeDualProblem::TaskSolveSingleTreeDualProblem(EnvironmentPtr env) : TaskBase(env) {

}
void TaskSolveSingleTreeDualProblem::initialize() {

}
void TaskSolveSingleTreeDualProblem::execute() {
  env_->mip_solver_->solveSingleTree();
}

}