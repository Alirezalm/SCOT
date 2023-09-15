//
// Created by alireza on 06/06/22.
//
//todo: try catch gurobi methods after the first implementation **IMPORTANT**

#include "MipSolverGurobiMultipleTree.h"
#include "../Solver/Utils.h"
#include "../Solver/Logger.h"

namespace Scot {

MipSolverGurobiMultipleTree::MipSolverGurobiMultipleTree(EnvironmentPtr env) {
  env_ = env;
  gathered_linear_outer_approximations_.resize(env_->Model->getNumberOfNodes());
  gathered_quadratic_outer_approximations_.resize(env_->Model->getNumberOfNodes());
}

void MipSolverGurobiMultipleTree::initializeModel() {
  try {
    gurobi_environment_ = std::make_shared<GRBEnv>();
    gurobi_model_ = std::make_shared<GRBModel>(gurobi_environment_.get());
  } catch (GRBException &e) {
    env_->Logger->logCritical("gurobi initialization error: " + e.getMessage(), env_->Model->getRank());
  }

}

void MipSolverGurobiMultipleTree::addVariable(std::string name, VariableType vtype, double lb, double ub) {

  try {
    switch (vtype) {
      case VariableType::BINARY: gurobi_model_->addVar(lb, ub, 0.0, GRB_BINARY, name);
        break;

      case VariableType::CONTINUOUS: gurobi_model_->addVar(lb, ub, 0.0, GRB_CONTINUOUS, name);
        break;

      default: break;
    }
  } catch (GRBException &e) {
    env_->Logger->logCritical("gurobi variable initialization error: " + e.getMessage(), env_->Model->getRank());
  }

}
void MipSolverGurobiMultipleTree::initializeObjectiveFunction() {

  try {
    gurobi_model_->update();
    objective_linear_expression_ = GRBLinExpr(0); // zero obj
  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi objective function initialization error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }
}
void MipSolverGurobiMultipleTree::addObjectiveLinearTerm(double coefficient, int variable_index) {
  try {
    objective_linear_expression_ += coefficient * gurobi_model_->getVar(variable_index);

  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi objective function adding linear term error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }
}
void MipSolverGurobiMultipleTree::addObjectiveConstantTerm(double constant) {
  try {
    objective_linear_expression_ += constant;
  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi objective function adding constant term error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }

}
void MipSolverGurobiMultipleTree::addObjectiveToModel() {
  // default behaviour is minimization
  try {
    gurobi_model_->setObjective(objective_linear_expression_, GRB_MINIMIZE);
  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi setting objective function error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }

}
void MipSolverGurobiMultipleTree::initializeConstraint() {

  try {
    constraint_linear_expression_ = GRBLinExpr(0);
    constraint_quadratic_expression_ = GRBQuadExpr(0);

  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi constraint initialization error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }
}
void MipSolverGurobiMultipleTree::addConstraintLinearTerm(double coefficient, int variable_index) {

  try {
    constraint_linear_expression_ += coefficient * gurobi_model_->getVar(variable_index);

  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi adding linear term to constraint error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }
}
void MipSolverGurobiMultipleTree::addConstraintQuadraticTerm(double coefficient,
                                                             int variable_index_1,
                                                             int variable_index_2) {
  try {
    constraint_quadratic_expression_ +=
        coefficient * gurobi_model_->getVar(variable_index_1) * gurobi_model_->getVar(variable_index_2);
  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi adding quadratic term to constraint error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }
}
void MipSolverGurobiMultipleTree::addConstraintToModel(double rhs) {

  assert(constraint_linear_expression_.size() > 0); // we assure that linear constraints exist

  if (constraint_quadratic_expression_.size() == 0) {

    try {
      gurobi_model_->addConstr(constraint_linear_expression_ <= rhs);
    } catch (GRBException &e) {
      env_->Logger->logCritical(
          "gurobi adding linear constraint to model error: " + e.getMessage(),
          env_->Model->getRank()
      );
    }

  } else {

    try {
      gurobi_model_->addQConstr(constraint_quadratic_expression_ + constraint_linear_expression_ <= rhs);

    } catch (GRBException &e) {
      env_->Logger->logCritical(
          "gurobi adding quadratic constraint to model error: " + e.getMessage(),
          env_->Model->getRank()
      );
    }
  }
}
void MipSolverGurobiMultipleTree::solve() {
  //todo: perform checks
  gurobi_model_->set("OutputFlag", "0"); //todo: for now important
  gurobi_model_->set("MIPGap", "1e-8");
//
  gurobi_model_->set("MIPGapAbs", "1e-8");
//  gurobi_model_->set("MIPFocus", "1");
//  gurobi_model_->set("NodeMethod", "1");
//  gurobi_model_->set("CutPasses", "100");
//  gurobi_model_->set("GomoryPasses", "100");
//  gurobi_model_->set("Threads", "1");
  try {
    gurobi_model_->optimize();
    auto stat = gurobi_model_->get(GRB_IntAttr_NumConstrs);
    auto status = gurobi_model_->get(GRB_IntAttr_Status);
    saveAllSolutions();
  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi solving model error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }

}
void MipSolverGurobiMultipleTree::addLinearOuterApproximation(LinearOuterApproximation linear_outer_approximation,
                                                              int node_index) {
  initializeConstraint();
  addConstraintLinearTerm(-1.0, node_index);
  for (int i = 0; i < env_->Model->getNumberOfVariables(); ++i) {
    addConstraintLinearTerm(linear_outer_approximation.local_gradient_at_feasible_point[i],
                            env_->Model->getNumberOfNodes() + i);
  }
  double right_hand_side = -linear_outer_approximation.local_objective_value_at_feasible_point + Utils::dot(
      linear_outer_approximation.local_gradient_at_feasible_point,
      linear_outer_approximation.consensus_feasible_point);

  addConstraintToModel(right_hand_side);

}

void MipSolverGurobiMultipleTree::addQuadraticOuterApproximation(QuadraticOuterApproximation quadratic_outer_approximation,
                                                                 int node_index) {
  initializeConstraint();
  addConstraintLinearTerm(-1.0, node_index);
  // multiple for loops just to test //todo: handle constraints in one loop
  for (int i = 0; i < env_->Model->getNumberOfVariables(); ++i) {
    addConstraintLinearTerm(quadratic_outer_approximation.local_gradient_at_feasible_point[i],
                            env_->Model->getNumberOfNodes() + i);
  }

  for (int i = 0; i < env_->Model->getNumberOfVariables(); ++i) {
    addConstraintQuadraticTerm(
        0.5 * quadratic_outer_approximation.local_min_eig_at_feasible_point,
        env_->Model->getNumberOfNodes() + i,
        env_->Model->getNumberOfNodes() + i
    );
    addConstraintLinearTerm(
        -quadratic_outer_approximation.local_min_eig_at_feasible_point
            * quadratic_outer_approximation.consensus_feasible_point[i],
        env_->Model->getNumberOfNodes() + i
    );
  }

  double right_hand_side = -quadratic_outer_approximation.local_objective_value_at_feasible_point + Utils::dot(
      quadratic_outer_approximation.local_gradient_at_feasible_point,
      quadratic_outer_approximation.consensus_feasible_point)
      - 0.5 * quadratic_outer_approximation.local_min_eig_at_feasible_point
          * Utils::dot(quadratic_outer_approximation.consensus_feasible_point,
                       quadratic_outer_approximation.consensus_feasible_point);

  addConstraintToModel(right_hand_side);
}

VectorDouble MipSolverGurobiMultipleTree::getBinarySolution() {
  VectorDouble bin_sol = std::vector<double>(env_->Model->getNumberOfVariables());
  try {

    for (int i = 0; i < bin_sol.size(); ++i) {
      bin_sol[i] = solution_[env_->Model->getNumberOfNodes() + env_->Model->getNumberOfVariables() + i];

    }
  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi getting binary solution error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }
  return bin_sol;
}
void MipSolverGurobiMultipleTree::saveAllSolutions() {

  try {
    int num_total_vars = gurobi_model_->get(GRB_IntAttr_NumVars);
    solution_ = std::vector<double>(num_total_vars, 0.0);
    for (int i = 0; i < num_total_vars; ++i) {
      solution_.at(i) = gurobi_model_->getVar(i).get(GRB_DoubleAttr_X);
    }
  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi getting all solutions error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }

}
VectorDouble MipSolverGurobiMultipleTree::getContinuesSolution() {
  VectorDouble cont_sol = std::vector<double>(env_->Model->getNumberOfVariables());
  try {
    for (int i = 0; i < cont_sol.size(); ++i) {
      cont_sol[i] = solution_[env_->Model->getNumberOfNodes() + i];
    }
  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi getting continuous solutions error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }
  return cont_sol;
}
double MipSolverGurobiMultipleTree::getObjectiveValue() {
  double obj_val;
  try {
    obj_val = gurobi_model_->get(GRB_DoubleAttr_ObjVal);

  } catch (GRBException &e) {
    env_->Logger->logCritical(
        "gurobi getting objective value error: " + e.getMessage(),
        env_->Model->getRank()
    );
  }
  return obj_val;
  // In case of SCP, obj is always 0 index
}
void MipSolverGurobiMultipleTree::gatherLinearOuterApproximation(LinearOuterApproximation linear_outer_approximation,
                                                                 int node_id) {
  //to make sure nodes save data in order
  gathered_linear_outer_approximations_.at(node_id) = linear_outer_approximation;
}
std::vector<LinearOuterApproximation> MipSolverGurobiMultipleTree::getTotalLinearOuterApproximations() {
  return gathered_linear_outer_approximations_;
}
void MipSolverGurobiMultipleTree::gatherQuadraticOuterApproximation(QuadraticOuterApproximation quadratic_outer_approximation,
                                                                    int node_id) {
  gathered_quadratic_outer_approximations_.at(node_id) = quadratic_outer_approximation;

}
std::vector<QuadraticOuterApproximation> MipSolverGurobiMultipleTree::getTotalQuadraticOuterApproximations() {
  return gathered_quadratic_outer_approximations_;
}
void MipSolverGurobiMultipleTree::solveSingleTree() {

}

}