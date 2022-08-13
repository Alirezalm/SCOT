//
// Created by alireza on 12/06/22.
//

#include "TaskSolveDualProblem.h"
#include "../MIPSolver/IMIPSolver.h"
namespace scot {

TaskSolveDualProblem::TaskSolveDualProblem(EnvironmentPtr env) : TaskBase(env) {

}
void TaskSolveDualProblem::initialize() {
//todo: checks
}
void TaskSolveDualProblem::execute() {

  env_->mip_solver_->solve();
}

}