//
// Created by alireza on 06/06/22.
//

#ifndef DISCOT_SRC_MIPSOLVER_IMIPSOLVER_H_
#define DISCOT_SRC_MIPSOLVER_IMIPSOLVER_H_
//todo: change interface based on needs

#include "../Solver/Environment.h"
namespace scot {

class IMipSolver {
 public:
  virtual ~IMipSolver() = default;

  virtual void initializeModel() = 0;
  virtual void addVariable(std::string name, VariableType vtype, double lb, double ub) = 0;

  virtual void initializeObjectiveFunction() = 0;
  virtual void addObjectiveLinearTerm(double coefficient, int variable_index) = 0;
  virtual void addObjectiveConstantTerm(double constant) = 0;
  virtual void addObjectiveToModel() = 0;

  virtual void initializeConstraint() = 0;
  virtual void addConstraintLinearTerm(double coefficient, int variable_index) = 0;
  virtual void addConstraintQuadraticTerm(double coefficient, int variable_index_1, int variable_index_2) = 0;
  virtual void addConstraintToModel(double rhs) = 0;

  virtual void addLinearOuterApproximation(LinearOuterApproximation linear_outer_approximation, int node_index) = 0;
  virtual void addQuadraticOuterApproximation(QuadraticOuterApproximation quadratic_outer_approximation, int node_index) = 0;

  //  virtual void addQuadraticOuterApproximation() = 0;

//  virtual void initializeMipSolverSettings() = 0;
  virtual void solve() = 0;
  virtual void solveSingleTree() = 0;
  virtual void saveAllSolutions() = 0;
  virtual VectorDouble getBinarySolution() = 0;
  virtual VectorDouble getContinuesSolution() = 0;
  virtual double getObjectiveValue() = 0;
  virtual void gatherLinearOuterApproximation(LinearOuterApproximation linear_outer_approximation, int node_id) = 0;
  virtual void gatherQuadraticOuterApproximation(QuadraticOuterApproximation quadratic_outer_approximation_outer_approximation, int node_id) = 0;

  virtual std::vector<LinearOuterApproximation> getTotalLinearOuterApproximations() = 0;
  virtual std::vector<QuadraticOuterApproximation> getTotalQuadraticOuterApproximations() = 0;
//  virtual std::vector<QuadraticOuterApproximation> getTotalLinearOuterApproximations() = 0;
  //virtual void getSolverStatus() = 0;
  //todo: add SOS-1 constraint
  //todo set solver options, timelimit, cutoff, ...
  //todo: get solver statistics



 protected:
  EnvironmentPtr env_;
  std::vector<LinearOuterApproximation> gathered_linear_outer_approximations_;
  std::vector<QuadraticOuterApproximation> gathered_quadratic_outer_approximations_;
};
using IMipSolverPtr = std::shared_ptr<IMipSolver>;
}

#endif //DISCOT_SRC_MIPSOLVER_IMIPSOLVER_H_
