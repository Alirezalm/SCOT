//
// Created by alireza on 20/06/22.
//

#ifndef SCOT_SRC_SOLVER_MESSAGEPASSINGINTERFACE_H_
#define SCOT_SRC_SOLVER_MESSAGEPASSINGINTERFACE_H_
// todo: voids -> bool and error check
#include "Environment.h"

namespace Scot {
class MessagePassingInterface {

public:
  explicit MessagePassingInterface(EnvironmentPtr env);
  ~MessagePassingInterface() = default;

  void gather(LinearOuterApproximation linearOuterApproximation);
  void gather(QuadraticOuterApproximation quadraticOuterApproximation);

  DualSolution bcast(DualSolution dualSolution);
  LinearOuterApproximation getLinearOuterApproximation(int nodeIndex);
  QuadraticOuterApproximation getQuadraticOuterApproximation(int nodeIndex);

private:
  EnvironmentPtr Env;
  VectorDouble MpiGatherRcvX;
  VectorDouble MpiGatherRcvGx;
  VectorDouble MpiGatherRcvObj;
  VectorDouble MpiGatherRcvMinEig;
  std::vector<LinearOuterApproximation> LinearOuterApproximations;
};
} // namespace Scot

#endif // SCOT_SRC_SOLVER_MESSAGEPASSINGINTERFACE_H_
