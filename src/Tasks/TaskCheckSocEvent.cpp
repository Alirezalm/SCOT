//
// Created by alireza on 07/07/22.
//

#include "TaskCheckSocEvent.h"
#include "../Solver/Results.h"
#include "TaskQueue.h"

namespace Scot {

TaskCheckSocEvent::TaskCheckSocEvent(EnvironmentPtr env) : TaskBase(env) {

}
void TaskCheckSocEvent::initialize() {

}
void TaskCheckSocEvent::execute() {
  //todo: error handling tasks finding
  auto current_abs_gap = env_->results_->getAbsoluteOptimalityGap();
  auto prev_abs_gap = env_->results_->best_abs_gap_pre_;

  auto gap_diff = abs(prev_abs_gap - current_abs_gap) / prev_abs_gap;

  auto target_task_gather_quad_oa = env_->task_queue_ptr_->getTask("t_gather_quad_oa");
  auto target_task_gather_linear_oa = env_->task_queue_ptr_->getTask("t_gather_lin_oa");

  auto target_task_add_quad_oa = env_->task_queue_ptr_->getTask("t_add_quadratic_oa_cut");
  auto target_task_add_linear_oa = env_->task_queue_ptr_->getTask("t_add_linear_oa_cut");

  if (gap_diff <= 1e-2) {//todo: read this from settings

    if (!target_task_gather_quad_oa.first->isActive()) target_task_gather_quad_oa.first->activate();
    if (target_task_gather_linear_oa.first->isActive()) target_task_gather_linear_oa.first->deactivate();

    if (!target_task_add_quad_oa.first->isActive()) target_task_add_quad_oa.first->activate();
    if (target_task_add_linear_oa.first->isActive()) target_task_add_linear_oa.first->deactivate();

    //todo: report when soc cut is using
  } else {
    if (target_task_gather_quad_oa.first->isActive()) target_task_gather_quad_oa.first->deactivate();
    if (!target_task_gather_linear_oa.first->isActive()) target_task_gather_linear_oa.first->activate();

    if (target_task_add_quad_oa.first->isActive()) target_task_add_quad_oa.first->deactivate();
    if (!target_task_add_linear_oa.first->isActive()) target_task_add_linear_oa.first->activate();
  }
}
}