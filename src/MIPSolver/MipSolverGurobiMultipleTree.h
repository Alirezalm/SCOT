//
// Created by alireza on 06/06/22.
//

#ifndef DISCOT_SRC_MIPSOLVER_MIPSOLVERGUROBIMULTIPLETREE_H_
#define DISCOT_SRC_MIPSOLVER_MIPSOLVERGUROBIMULTIPLETREE_H_

#include "../Solver/Environment.h"
#include "IMIPSolver.h"
#include "gurobi_c++.h"

namespace Scot {

class MipSolverGurobiMultipleTree : public IMipSolver {
 public:

  explicit MipSolverGurobiMultipleTree(EnvironmentPtr env);
  ~MipSolverGurobiMultipleTree() override = default;

  void initializeModel() override;
  void addVariable(std::string name, VariableType vtype, double lb, double ub) override;

  void initializeObjectiveFunction() override;
  void addObjectiveLinearTerm(double coefficient, int variable_index) override;
  void addObjectiveConstantTerm(double constant) override;
  void addObjectiveToModel() override;

  void initializeConstraint() override;
  void addConstraintLinearTerm(double coefficient, int variable_index) override;
  void addConstraintQuadraticTerm(double coefficient, int variable_index_1, int variable_index_2) override;
  void addConstraintToModel(double rhs) override;

  void addLinearOuterApproximation(LinearOuterApproximation linear_outer_approximation, int node_index) override;
  void addQuadraticOuterApproximation(QuadraticOuterApproximation quadratic_outer_approximation, int node_index) override;
  void solve() override;
  void solveSingleTree() override;
  VectorDouble getBinarySolution() override;
  VectorDouble getContinuesSolution() override;
  double getObjectiveValue() override;

  void gatherLinearOuterApproximation(LinearOuterApproximation linear_outer_approximation, int node_id) override;
  std::vector<LinearOuterApproximation> getTotalLinearOuterApproximations() override;

  void gatherQuadraticOuterApproximation(QuadraticOuterApproximation quadratic_outer_approximation,
                                         int node_id) override;
  std::vector<QuadraticOuterApproximation> getTotalQuadraticOuterApproximations() override;

  std::shared_ptr<GRBModel> gurobi_model_;

 private:
  void saveAllSolutions() override;
  std::shared_ptr<GRBEnv> gurobi_environment_;
  GRBLinExpr objective_linear_expression_; //can also be quadratic if needed
  GRBLinExpr constraint_linear_expression_;
  GRBQuadExpr constraint_quadratic_expression_;
  VectorDouble solution_;
};

}

#endif //DISCOT_SRC_MIPSOLVER_MIPSOLVERGUROBIMULTIPLETREE_H_
