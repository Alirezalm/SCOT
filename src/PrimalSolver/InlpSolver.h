//
// Created by alireza on 30/05/22.
//
// TODO: Saving primal results to file?

#ifndef DISCOT_SRC_PRIMALSOLVER_INLPSOLVER_H_
#define DISCOT_SRC_PRIMALSOLVER_INLPSOLVER_H_

#include "../Solver/Environment.h"
namespace scot {

class InlpSolver {

 public:
  virtual ~InlpSolver() = default;
  virtual void setFixedBinaryVariables(VectorDouble &binary_vec) = 0;
  virtual void solveProblem() = 0;
  virtual IncumbentSolution getPrimalSolution() = 0;

 protected:
  EnvironmentPtr env_;
  IncumbentSolution primal_solution_;
  VectorDouble binary_vec_;
};
using InlpSolverPtr = std::shared_ptr<InlpSolver>;
}

#endif //DISCOT_SRC_PRIMALSOLVER_INLPSOLVER_H_
