//
// Created by alireza on 30/05/22.
//

#include "DistributedOperatorSplittingEngine.h"
#include "include/types.h"
// TODO: PRIMAL SETTINGS MISSED

namespace scot {

DistributedOperatorSplittingEngine::DistributedOperatorSplittingEngine(EnvironmentPtr env) {
  env_ = env;

  distributed_solver_ = std::make_shared<dose::DOSE>(
      env->model_->getObjectiveFunction()->getSamplingData(),
      env->model_->getObjectiveFunction()->getResponseData(),
      env_->model_->getProblemType(),
      env->model_->getRank(),
      env->model_->getNumberOfNodes(),
      env->model_->getBigMParam()
  );
  auto settings = std::make_shared<dose::RHADMMSettings>();
  settings->verbose = false;//todo: for test IMPORTANT
  settings->eps = 1e-10;//todo: for test IMPORTANT
  distributed_solver_->setSettings(
      settings //todo: for test IMPORTANT
  );
}

void DistributedOperatorSplittingEngine::setFixedBinaryVariables(VectorDouble &binary_vec) {
  binary_vec_ = binary_vec;
}

void DistributedOperatorSplittingEngine::solveProblem() {
  //TODO: user defined casting can be here?
  //TODO: error handling for solve method down here
  //todo: provide settings for dose

  distributed_solver_->solve(binary_vec_);
  auto dose_solution = distributed_solver_->GetSolution();
  primal_solution_.consensus_solution = dose_solution.x;
  primal_solution_.local_gradient = dose_solution.gx;
  primal_solution_.local_obj_value = dose_solution.local_f;
  primal_solution_.total_obj_value = dose_solution.total_f;
  primal_solution_.min_eig_val = dose_solution.minEig;
  primal_solution_.status = dose_solution.status;

}

IncumbentSolution DistributedOperatorSplittingEngine::getPrimalSolution() {
  return primal_solution_;
}
}