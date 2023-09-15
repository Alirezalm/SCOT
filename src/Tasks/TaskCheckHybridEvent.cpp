//
// Created by alireza on 12/07/22.
//

#include "TaskCheckHybridEvent.h"
#include "../Solver/Results.h"
#include "TaskQueue.h"
#include "TaskDistributedNlpSolution.h"
#include "TaskGatherLocalQuadraticOuterApproximations.h"
#include "TaskCreateMultipleTreeDualProblem.h"
#include "TaskAddQuadraticOuterApproximation.h"

namespace Scot {

TaskCheckHybridEvent::TaskCheckHybridEvent(EnvironmentPtr env) : TaskBase(env) {

}
void TaskCheckHybridEvent::initialize() {

}
void TaskCheckHybridEvent::execute() {
  auto current_abs_gap = env_->Results->getAbsoluteOptimalityGap();
  auto prev_abs_gap = env_->Results->best_abs_gap_pre_;

  auto gap_diff = abs(prev_abs_gap - current_abs_gap) / prev_abs_gap;
  if (gap_diff <=1e-1) { //todo: read from settings or provide a mechanism
    auto mt_solver = env_->TaskQueue->getTask("t_solve_dual_problem");
    mt_solver.first->deactivate();

    auto single_tree_solver_task_pair = env_->TaskQueue->getTask("t_solve_dual_single_tree");
    single_tree_solver_task_pair.first->activate();
  }

}
}