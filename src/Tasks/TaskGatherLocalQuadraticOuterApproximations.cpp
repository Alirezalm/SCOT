//
// Created by alireza on 01/07/22.
//

#include "TaskGatherLocalQuadraticOuterApproximations.h"
#include "../Solver/Logger.h"
#include "../Solver/Results.h"
#include "../Solver/MessagePassingInterface.h"
#include "../MIPSolver/IMIPSolver.h"
namespace Scot {

TaskGatherLocalQuadraticOuterApproximations::TaskGatherLocalQuadraticOuterApproximations(EnvironmentPtr env) : TaskBase(env) {

}
void TaskGatherLocalQuadraticOuterApproximations::initialize() {

}
void TaskGatherLocalQuadraticOuterApproximations::execute() {
  env_->Logger->logDebug("getting local incumbent to scatter.", env_->Model->getRank());
  auto current_incumbent = env_->Results->getCurrentIncumbent();
  auto quadratic_oa = QuadraticOuterApproximation{
      current_incumbent.consensus_solution,
      current_incumbent.local_gradient,
      current_incumbent.local_obj_value,
      current_incumbent.min_eig_val
  };
  env_->Logger->logDebug("performing MPI gather local linear outer approximations ", env_->Model->getRank());
  env_->Mpi->gather(quadratic_oa);


    for (int i = 0; i < env_->Model->getNumberOfNodes(); ++i) {
      auto local_oa = env_->Mpi->getQuadraticOuterApproximation(i);
      env_->MipSolver->gatherQuadraticOuterApproximation(local_oa, i);
    }
}
}