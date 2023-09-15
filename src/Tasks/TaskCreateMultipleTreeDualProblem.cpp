//
// Created by alireza on 08/06/22.
//

#include "TaskCreateMultipleTreeDualProblem.h"
#include "../MIPSolver/MipSolverGurobiMultipleTree.h"
#include "gurobi_c++.h"
#include "../ExternLibs/fmt/include/core.h"

namespace Scot {

TaskCreateMultipleTreeDualProblem::TaskCreateMultipleTreeDualProblem(EnvironmentPtr env) : TaskBase(env) {
  // creating invariant parts of scp master problem here: multiple tree version

  //initializing model
  env_->MipSolver->initializeModel();

  //todo: replace grb_infty with DISCOT infty

  for (int i = 0; i < env_->Model->getNumberOfNodes(); ++i) {
    std::string gama_var_name = fmt::format("gamma_{}", i);
    env_->MipSolver->addVariable(gama_var_name, VariableType::CONTINUOUS, -GRB_INFINITY,
                                                 GRB_INFINITY);
  }
  for (int i = 0; i < env_->Model->getNumberOfVariables(); ++i) {
    std::string node_var_name = fmt::format("x_{}", i);
    env_->MipSolver->addVariable(node_var_name, VariableType::CONTINUOUS, -GRB_INFINITY,
                                                 GRB_INFINITY);
  }

  for (int i = 0; i < env_->Model->getNumberOfVariables(); ++i) {
    std::string lfc_var_name = fmt::format("delta_{}", i);
    env_->MipSolver->addVariable(lfc_var_name, VariableType::BINARY, -GRB_INFINITY,
                                                 GRB_INFINITY);
  }



  //initializing objective function
  env_->MipSolver->initializeObjectiveFunction();

  // creating objective function
  for (int i = 0; i < env_->Model->getNumberOfNodes(); ++i) {
    env_->MipSolver->addObjectiveLinearTerm(1.0, i); // sum of gammas in the objective function
  }

  env_->MipSolver->addObjectiveToModel();

  // creating big-m constraints -inf <= x - M * delta <= 0
  for (int i = 0; i < env_->Model->getNumberOfVariables(); ++i) {
    env_->MipSolver->initializeConstraint();
    env_->MipSolver->addConstraintLinearTerm(1.0, env_->Model->getNumberOfNodes() + i);
    env_->MipSolver->addConstraintLinearTerm(-env_->Model->getBigMParam(),
                                                             env_->Model->getNumberOfNodes()
                                                                 + env_->Model->getNumberOfVariables() + i);
    env_->MipSolver->addConstraintToModel(0.0);

  }
  // creating big-m constraints  -x - M * delta <= 0
  for (int i = 0; i < env_->Model->getNumberOfVariables(); ++i) {
    env_->MipSolver->initializeConstraint();
    env_->MipSolver->addConstraintLinearTerm(-1.0, env_->Model->getNumberOfNodes() + i);
    env_->MipSolver->addConstraintLinearTerm(-env_->Model->getBigMParam(),
                                                             env_->Model->getNumberOfNodes()
                                                                 + env_->Model->getNumberOfVariables() + i);
    env_->MipSolver->addConstraintToModel(0.0);

  }
  //creating cardinality constraint sum delta_i <= kappa
  env_->MipSolver->initializeConstraint();
  for (int i = 0; i < env_->Model->getNumberOfVariables(); ++i) {
    env_->MipSolver->addConstraintLinearTerm(1.0,
                                                             env_->Model->getNumberOfNodes()
                                                                 + env_->Model->getNumberOfVariables() + i);
  }
  env_->MipSolver->addConstraintToModel(env_->Model->getNumberOfNonzeros());
}

void TaskCreateMultipleTreeDualProblem::initialize() {

}

void TaskCreateMultipleTreeDualProblem::execute() {

}

}