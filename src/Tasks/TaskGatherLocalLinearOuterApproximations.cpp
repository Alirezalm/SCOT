//
// Created by alireza on 20/06/22.
//

#include "TaskGatherLocalLinearOuterApproximations.h"
#include "../Solver/Logger.h"
#include "../Solver/Results.h"
#include "../Solver/MessagePassingInterface.h"
#include "../MIPSolver/IMIPSolver.h"

namespace scot {

TaskGatherLocalLinearOuterApproximations::TaskGatherLocalLinearOuterApproximations(EnvironmentPtr env) : TaskBase(env) {

}
void TaskGatherLocalLinearOuterApproximations::initialize() {

}
void TaskGatherLocalLinearOuterApproximations::execute() {

  auto current_incumbent = env_->results_->getCurrentIncumbent();
  env_->logger_->logDebug("getting local incumbent to scatter.", env_->model_->getRank());
  auto linear_oa = LinearOuterApproximation{current_incumbent.consensus_solution,
                                            current_incumbent.local_gradient,
                                            current_incumbent.local_obj_value};

  env_->logger_->logDebug("performing MPI gather local linear outer approximations ", env_->model_->getRank());
  env_->mpi_->gather(linear_oa);

  for (int i = 0; i < env_->model_->getNumberOfNodes(); ++i) {
    auto local_oa = env_->mpi_->getLinearOuterApproximation(i);
    env_->mip_solver_->gatherLinearOuterApproximation(local_oa, i);
  }

}
}