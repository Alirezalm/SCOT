//
// Created by alireza on 02/06/22.
//

#include "Iteration.h"
#include "Results.h"
#include "Logger.h"

namespace Scot {
Iteration::Iteration(EnvironmentPtr env) {
  env_ = env;
  iteration_number_ = env_->results_->getNumberOfIterations() + 1;

  if (iteration_number_ != 1) {

    total_number_of_cuts_ = env_->results_->iterations_.back()->total_number_of_cuts_;
    number_first_order_oa_cuts_ = env->results_->iterations_.back()->number_first_order_oa_cuts_;

    number_second_order_oa_cuts_ = env->results_->iterations_.back()->number_second_order_oa_cuts_;
    number_ecp_cuts_ = env->results_->iterations_.back()->number_ecp_cuts_;
  }

  env_->logger_->logDebug("new iteration created.", env_->model_->getRank());
  iteration_time_ = 1e-3; //todo: need revise

}
}

