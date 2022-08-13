//
// Created by alireza on 25/05/22.
//
#include "Model.h"

namespace scot {

Model::Model(int number_of_nodes, int rank, double big_m_param) {
  assert(number_of_nodes > 0);
  assert(rank >= 0);
  number_of_nodes_ = number_of_nodes;
  rank_ = rank;
  big_m_param_ = big_m_param;
  number_of_nonzeros_ = 0;
  number_of_variables_ = 0;
}
IObjectivePtr Model::getObjectiveFunction() {
  assert(objective_function_->getNumberOfRows() > 0 && objective_function_->getNumberOfColumns() > 0);
  return objective_function_;
}
void Model::setObjectiveFunction(IObjectivePtr new_objective_function) {
  assert(new_objective_function->getNumberOfRows() > 0 && new_objective_function->getNumberOfColumns() > 0);
  objective_function_ = new_objective_function;
  number_of_variables_ = objective_function_->getNumberOfColumns();
}
SparsityConstraintPtr Model::getSparsityConstraint() {
  return sparsity_constraint_;
}
void Model::setSparsityConstraint(SparsityConstraintPtr new_sparsity_constraint) {
  assert(new_sparsity_constraint->getNumberOfNonZeros() > 0);
  assert(new_sparsity_constraint->getNumberOfNonZeros() < objective_function_->getNumberOfColumns());
  sparsity_constraint_ = new_sparsity_constraint;
  number_of_nonzeros_ = sparsity_constraint_->getNumberOfNonZeros();
}
int Model::getNumberOfVariables() {
  return number_of_variables_;
}
int Model::getNumberOfNonzeros() {
  return number_of_nonzeros_;
}
int Model::getNumberOfNodes() {
  return number_of_nodes_;
}
dose::ProblemType Model::getProblemType() {
  return objective_function_->getObjectiveType();
}
double Model::getBigMParam() {
  return big_m_param_;
}
int Model::getRank() {
  return rank_;
}

}