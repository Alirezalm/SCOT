//
// Created by alireza on 30/05/22.
//

#ifndef DISCOT_SRC_PRIMALSOLVER_DISTRIBUTEDOPERATORSPLITTINGENGINE_H_
#define DISCOT_SRC_PRIMALSOLVER_DISTRIBUTEDOPERATORSPLITTINGENGINE_H_

#include "InlpSolver.h"
#include "memory"

namespace Scot {

class DistributedOperatorSplittingEngine : public InlpSolver {

 public:
  DistributedOperatorSplittingEngine(EnvironmentPtr env);
  ~DistributedOperatorSplittingEngine() override = default;

  void setFixedBinaryVariables(VectorDouble &binary_vec) override;
  void solveProblem() override;

  IncumbentSolution getPrimalSolution() override;

 private:
  DosePtr distributed_solver_;

};

}

#endif //DISCOT_SRC_PRIMALSOLVER_DISTRIBUTEDOPERATORSPLITTINGENGINE_H_
