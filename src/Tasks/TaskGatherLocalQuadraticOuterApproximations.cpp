//
// Created by alireza on 01/07/22.
//

#include "TaskGatherLocalQuadraticOuterApproximations.h"
#include "../Solver/Logger.h"
#include "../Solver/Results.h"
#include "../Solver/MessagePassingInterface.h"
#include "../MIPSolver/IMIPSolver.h"
namespace scot {

TaskGatherLocalQuadraticOuterApproximations::TaskGatherLocalQuadraticOuterApproximations(EnvironmentPtr env) : TaskBase(env) {

}
void TaskGatherLocalQuadraticOuterApproximations::initialize() {

}
void TaskGatherLocalQuadraticOuterApproximations::execute() {
  env_->logger_->logDebug("getting local incumbent to scatter.", env_->model_->getRank());
  auto current_incumbent = env_->results_->getCurrentIncumbent();
  auto quadratic_oa = QuadraticOuterApproximation{
      current_incumbent.consensus_solution,
      current_incumbent.local_gradient,
      current_incumbent.local_obj_value,
      current_incumbent.min_eig_val
  };
  env_->logger_->logDebug("performing MPI gather local linear outer approximations ", env_->model_->getRank());
  env_->mpi_->gather(quadratic_oa);


    for (int i = 0; i < env_->model_->getNumberOfNodes(); ++i) {
      auto local_oa = env_->mpi_->getQuadraticOuterApproximation(i);
      env_->mip_solver_->gatherQuadraticOuterApproximation(local_oa, i);
    }
}
}