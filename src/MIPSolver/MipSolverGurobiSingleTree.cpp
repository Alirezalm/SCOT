//
// Created by alireza on 11/07/22.
//
//todo: MUST BE REFACORED ** IMPORTANT **
#include "MipSolverGurobiSingleTree.h"
#include "../Solver/Logger.h"
#include "../Tasks/TaskQueue.h"
#include "../Solver/Results.h"
#include "../Tasks/TaskGatherLocalLinearOuterApproximations.h"
#include "../Tasks/TaskAddLinearOuterApproximation.h"
#include "../Tasks/TaskGatherLocalLinearOuterApproximations.h"
#include "../Solver/Utils.h"
namespace scot {

MipGurobiCallback::MipGurobiCallback(GRBVar *xvars, EnvironmentPtr env) {
  env_ = env;
  vars = xvars;
  total_vars_ = env_->model_->getNumberOfVariables() * 2 + env_->model_->getNumberOfNodes();
  current_integer_.resize(env_->model_->getNumberOfVariables());
  current_real_.resize(env_->model_->getNumberOfVariables());
  current_objval_ = 0;
}

void MipGurobiCallback::callback() {
  try {

    if (where == GRB_CB_MIPSOL) {
      auto dual_solution = getDualSolution();
      auto best_primal = env_->results_->getBestIncumbentBound();

      if (dual_solution.objective_value > best_primal) {
        return;
      }

      if (best_primal - dual_solution.objective_value <= 1e-5) {//todo: recieve from user
        abort();
        return;
      }
      env_->results_->addDualSolution(dual_solution);
      solveLazyDistributedNlp();
      auto task_gather_linear_oa = std::make_shared<TaskGatherLocalLinearOuterApproximations>(env_);
      task_gather_linear_oa->execute();

      addLazyConstraint();

    }

  } catch (GRBException &e) {
    env_->logger_->logCritical(e.getMessage(), env_->model_->getRank());
  }
}
void MipGurobiCallback::updateCurrentIntegerIncumbent() {
  for (int i = 0; i < env_->model_->getNumberOfVariables(); ++i) {
    current_integer_[i] =
        current_incumbent_[env_->model_->getNumberOfNodes() + env_->model_->getNumberOfVariables() + i];
  }
}
void MipGurobiCallback::updateCurrentIncumbent() {

  current_incumbent_.resize(total_vars_);
  for (int i = 0; i < total_vars_; ++i) {
    current_incumbent_.at(i) = getSolution(vars[i]);
  }
}
void MipGurobiCallback::updateCurrentRealIncumbent() {
  for (int i = 0; i < env_->model_->getNumberOfVariables(); ++i) {
    current_real_[i] =
        current_incumbent_[env_->model_->getNumberOfNodes() + i];
  }

}
DualSolution MipGurobiCallback::getDualSolution() {
  updateCurrentIncumbent();
  updateCurrentIntegerIncumbent();
  updateCurrentRealIncumbent();
  updateCurrentObjective();
  DualSolution dual_solution = {
      current_integer_,
      current_real_,
      current_objval_
  };
  return dual_solution;
}
void MipGurobiCallback::updateCurrentObjective() {
  current_objval_ = getDoubleInfo(GRB_CB_MIPSOL_OBJ);
}
void MipGurobiCallback::solveLazyDistributedNlp() {
  auto task_solve_nlp = env_->task_queue_ptr_->getTask("t_solve_nlp");
  task_solve_nlp.first->execute();
}
void MipGurobiCallback::addLazyConstraint() {
  auto coeff = -1.0;
  for (int i = 0; i < env_->model_->getNumberOfNodes(); ++i) {
    GRBLinExpr expr = 0.0;

    auto lin_oa = env_->mip_solver_->getTotalLinearOuterApproximations().at(i);

    expr += coeff * vars[i];
    for (int j = 0; j < env_->model_->getNumberOfVariables(); ++j) {
      expr += lin_oa.local_gradient_at_feasible_point[j] * vars[env_->model_->getNumberOfNodes() + j];
    }
    auto rhs = -lin_oa.local_objective_value_at_feasible_point + Utils::dot(
        lin_oa.local_gradient_at_feasible_point,
        lin_oa.consensus_feasible_point
    );
    addLazy(expr <= rhs);
  }

}
MipSolverGurobiSingleTree::MipSolverGurobiSingleTree(EnvironmentPtr env) : MipSolverGurobiMultipleTree(env) {}

void MipSolverGurobiSingleTree::solveSingleTree() {
  if (env_->settings_->getDblSetting("verbose") == 1) {
    gurobi_model_->set("OutputFlag", "1");
  } else {
    gurobi_model_->set("OutputFlag", "0");
  }
  gurobi_model_->set("TimeLimit", std::to_string(env_->settings_->getDblSetting("max_time")));
  gurobi_model_->set(GRB_IntParam_LazyConstraints, 1);
  MipGurobiCallback cb = MipGurobiCallback(gurobi_model_->getVars(), env_);
  gurobi_model_->setCallback(&cb);
  auto stat = gurobi_model_->get(GRB_IntAttr_NumConstrs);

  gurobi_model_->optimize();
  //todo: must be changed in future
  auto status = gurobi_model_->get(GRB_IntAttr_Status);
  if (status == GRB_TIME_LIMIT) {
    MPI_Barrier(MPI_COMM_WORLD);
    throw std::out_of_range("gurobi single tree: maximum time limit reached.\n");
  }
}

}