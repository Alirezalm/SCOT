//
// Created by alireza on 11/07/22.
//

#ifndef SCOT_SRC_MIPSOLVER_MIPSOLVERGUROBISINGLETREE_H_
#define SCOT_SRC_MIPSOLVER_MIPSOLVERGUROBISINGLETREE_H_
#include <gurobi_c++.h>
#include "../Solver/Environment.h"
#include "MipSolverGurobiMultipleTree.h"
namespace Scot {

class MipGurobiCallback : public GRBCallback {

 public:
  GRBVar *vars;
  explicit MipGurobiCallback(GRBVar *xvars, EnvironmentPtr env);
  ~MipGurobiCallback() override = default;

 protected:
  void callback() override;
 private:
  void updateCurrentIncumbent();
  void updateCurrentIntegerIncumbent();
  void updateCurrentRealIncumbent();
  void updateCurrentObjective();
  void solveLazyDistributedNlp();
  void addLazyConstraint();
  DualSolution getDualSolution();

  VectorDouble current_incumbent_;
  VectorDouble current_real_;
  VectorDouble current_integer_;
  double current_objval_;
  int total_vars_;
  EnvironmentPtr env_;
};

class MipSolverGurobiSingleTree : public MipSolverGurobiMultipleTree {

 public:
  explicit MipSolverGurobiSingleTree(EnvironmentPtr env);
  ~MipSolverGurobiSingleTree() override = default;

  void solveSingleTree() override;
 private:

 protected:

};

}

#endif //SCOT_SRC_MIPSOLVER_MIPSOLVERGUROBISINGLETREE_H_
