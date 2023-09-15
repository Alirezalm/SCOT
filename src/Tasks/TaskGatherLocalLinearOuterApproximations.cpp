//
// Created by alireza on 20/06/22.
//

#include "TaskGatherLocalLinearOuterApproximations.h"
#include "../Solver/Logger.h"
#include "../Solver/Results.h"
#include "../Solver/MessagePassingInterface.h"
#include "../MIPSolver/IMIPSolver.h"

namespace Scot {

TaskGatherLocalLinearOuterApproximations::TaskGatherLocalLinearOuterApproximations(EnvironmentPtr env) : TaskBase(env) {

}
void TaskGatherLocalLinearOuterApproximations::initialize() {

}
void TaskGatherLocalLinearOuterApproximations::execute() {

  auto current_incumbent = env_->Results->getCurrentIncumbent();
  env_->Logger->logDebug("getting local incumbent to scatter.", env_->Model->getRank());
  auto linear_oa = LinearOuterApproximation{current_incumbent.consensus_solution,
                                            current_incumbent.local_gradient,
                                            current_incumbent.local_obj_value};

  env_->Logger->logDebug("performing MPI gather local linear outer approximations ", env_->Model->getRank());
  env_->Mpi->gather(linear_oa);

  for (int i = 0; i < env_->Model->getNumberOfNodes(); ++i) {
    auto local_oa = env_->Mpi->getLinearOuterApproximation(i);
    env_->MipSolver->gatherLinearOuterApproximation(local_oa, i);
  }

}
}