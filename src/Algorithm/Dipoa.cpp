//
// Created by alireza on 30/05/22.
//

#include "Dipoa.h"
#include "../Solver/Results.h"
#include "../Solver/Logger.h"
#include "../Solver/Report.h"
#include "../Solver/Timer.h"
#include "../PrimalSolver/DistributedOperatorSplittingEngine.h"
#include "../Tasks/TaskManager.h"
#include "../Tasks/TaskDistributedNlpSolution.h"
#include "../Tasks/TaskInitializeNewIteration.h"
#include "../Tasks/TaskInitializeMultipleTreeDualSolver.h"
#include "../Tasks/TaskCreateMultipleTreeDualProblem.h"
#include "../Tasks/TaskAddLinearOuterApproximation.h"
#include "../Tasks/TaskSolveDualProblem.h"
#include "../Tasks/TaskAddDualSolution.h"
#include "../Tasks/TaskGatherLocalLinearOuterApproximations.h"
#include "../Tasks/TaskBcastDualSolution.h"
#include "../Tasks/TaskPrintIterationInfo.h"
#include "../Tasks/TaskComputeObjectiveGap.h"
#include "../Tasks/TaskCheckTerminationGap.h"
#include "../Tasks/TaskGatherLocalQuadraticOuterApproximations.h"
#include "../Tasks/TaskAddQuadraticOuterApproximation.h"
#include "../Tasks/TaskCheckSocEvent.h"
#include "../Tasks/TaskCheckDuration.h"

namespace scot {
//todo: implenent different switching between soc and foc based on settings

Dipoa::Dipoa(EnvironmentPtr env) {
  env_ = env;

// adding tasks to the task queue
  auto task_initialize_new_iteration = std::make_shared<TaskInitializeNewIteration>(env_);
  env_->task_manager_->addTask(task_initialize_new_iteration, "t_initialize_new_iteration");

  auto task_initialize_dual_solver = std::make_shared<TaskInitializeMultipleTreeDualSolver>(env_);
  env_->task_manager_->addTask(task_initialize_dual_solver, "t_initialize_dual_solver");

  auto task_solve_nlp = std::make_shared<TaskDistributedNlpSolution>(env_);
  env_->task_manager_->addTask(task_solve_nlp, "t_solve_nlp");

  auto task_gather_linear_oa = std::make_shared<TaskGatherLocalLinearOuterApproximations>(env_);
  env_->task_manager_->addTask(task_gather_linear_oa, "t_gather_lin_oa");
//
  auto task_gather_quadratic_oa = std::make_shared<TaskGatherLocalQuadraticOuterApproximations>(env_);
  env_->task_manager_->addTask(task_gather_quadratic_oa, "t_gather_quad_oa");
  task_gather_quadratic_oa->deactivate();
//
  auto task_create_dual_problem = std::make_shared<TaskCreateMultipleTreeDualProblem>(env_);
  env_->task_manager_->addTask(task_create_dual_problem, "t_create_dual_problem");
//
  auto task_add_linear_oa_cut = std::make_shared<TaskAddLinearOuterApproximation>(env_);
  env_->task_manager_->addTask(task_add_linear_oa_cut, "t_add_linear_oa_cut");
//
  auto task_add_quadratic_oa_cut = std::make_shared<TaskAddQuadraticOuterApproximation>(env_);
  env_->task_manager_->addTask(task_add_quadratic_oa_cut, "t_add_quadratic_oa_cut");
  task_add_quadratic_oa_cut->deactivate();
//
  auto task_solve_dual_problem = std::make_shared<TaskSolveDualProblem>(env_);
  env_->task_manager_->addTask(task_solve_dual_problem, "t_solve_dual_problem");
//
  auto task_add_dual_solution = std::make_shared<TaskAddDualSolution>(env_);
  env_->task_manager_->addTask(task_add_dual_solution, "t_add_dual_solution");
//
  auto task_bcast_dual_solution = std::make_shared<TaskBcastDualSolution>(env_);
  env->task_manager_->addTask(task_bcast_dual_solution, "t_bcast_dual_solution");
  task_bcast_dual_solution->deactivate();
//
  auto task_compute_gaps = std::make_shared<TaskComputeObjectiveGap>(env_);
  env->task_manager_->addTask(task_compute_gaps, "t_compute_gaps");
//
  if (env_->settings_->getDblSetting("verbose") == 1) {
    auto task_print_iteration = std::make_shared<TaskPrintIterationInfo>(env_);
    env_->task_manager_->addTask(task_print_iteration, "t_print_iteration");
  }

  auto task_check_soc_event = std::make_shared<TaskCheckSocEvent>(env_);
  env_->task_manager_->addTask(task_check_soc_event, "t_check_soc_event");
//
  auto task_check_termination_gap = std::make_shared<TaskCheckTerminationGap>(env_);
  env_->task_manager_->addTask(task_check_termination_gap, "t_check_termination_gap");

  auto task_check_duration = std::make_shared<TaskCheckDuration>(env_);
  env_->task_manager_->addTask(task_check_duration, "t_check_duration");

}
bool scot::Dipoa::Run() {
  if (env_->settings_->getDblSetting("verbose") == 1) {
    outputIterHeader();
  }
  try {
    while (!env_->task_manager_->isTaskQueueEmpty()) {
      for (const auto &task : env_->task_manager_->tasks_) {
        if (task.first->isActive()) {
          task.first->execute();
        }
      }
    }
    return true;
  } catch (std::exception &exception) {
    env_->logger_->logWarning(exception.what(), env_->model_->getRank());
    return false;
  }

}
void Dipoa::outputIterHeader() {
  if (env_->model_->getRank() == 0) {
    env_->report_->printIterationHeader();
  }
}
}
