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
  env_->mip_solver_->initializeModel();

  //todo: replace grb_infty with DISCOT infty

  for (int i = 0; i < env_->model_->getNumberOfNodes(); ++i) {
    std::string gama_var_name = fmt::format("gamma_{}", i);
    env_->mip_solver_->addVariable(gama_var_name, VariableType::CONTINUOUS, -GRB_INFINITY,
                                                 GRB_INFINITY);
  }
  for (int i = 0; i < env_->model_->getNumberOfVariables(); ++i) {
    std::string node_var_name = fmt::format("x_{}", i);
    env_->mip_solver_->addVariable(node_var_name, VariableType::CONTINUOUS, -GRB_INFINITY,
                                                 GRB_INFINITY);
  }

  for (int i = 0; i < env_->model_->getNumberOfVariables(); ++i) {
    std::string lfc_var_name = fmt::format("delta_{}", i);
    env_->mip_solver_->addVariable(lfc_var_name, VariableType::BINARY, -GRB_INFINITY,
                                                 GRB_INFINITY);
  }



  //initializing objective function
  env_->mip_solver_->initializeObjectiveFunction();

  // creating objective function
  for (int i = 0; i < env_->model_->getNumberOfNodes(); ++i) {
    env_->mip_solver_->addObjectiveLinearTerm(1.0, i); // sum of gammas in the objective function
  }

  env_->mip_solver_->addObjectiveToModel();

  // creating big-m constraints -inf <= x - M * delta <= 0
  for (int i = 0; i < env_->model_->getNumberOfVariables(); ++i) {
    env_->mip_solver_->initializeConstraint();
    env_->mip_solver_->addConstraintLinearTerm(1.0, env_->model_->getNumberOfNodes() + i);
    env_->mip_solver_->addConstraintLinearTerm(-env_->model_->getBigMParam(),
                                                             env_->model_->getNumberOfNodes()
                                                                 + env_->model_->getNumberOfVariables() + i);
    env_->mip_solver_->addConstraintToModel(0.0);

  }
  // creating big-m constraints  -x - M * delta <= 0
  for (int i = 0; i < env_->model_->getNumberOfVariables(); ++i) {
    env_->mip_solver_->initializeConstraint();
    env_->mip_solver_->addConstraintLinearTerm(-1.0, env_->model_->getNumberOfNodes() + i);
    env_->mip_solver_->addConstraintLinearTerm(-env_->model_->getBigMParam(),
                                                             env_->model_->getNumberOfNodes()
                                                                 + env_->model_->getNumberOfVariables() + i);
    env_->mip_solver_->addConstraintToModel(0.0);

  }
  //creating cardinality constraint sum delta_i <= kappa
  env_->mip_solver_->initializeConstraint();
  for (int i = 0; i < env_->model_->getNumberOfVariables(); ++i) {
    env_->mip_solver_->addConstraintLinearTerm(1.0,
                                                             env_->model_->getNumberOfNodes()
                                                                 + env_->model_->getNumberOfVariables() + i);
  }
  env_->mip_solver_->addConstraintToModel(env_->model_->getNumberOfNonzeros());
}

void TaskCreateMultipleTreeDualProblem::initialize() {

}

void TaskCreateMultipleTreeDualProblem::execute() {

}

}