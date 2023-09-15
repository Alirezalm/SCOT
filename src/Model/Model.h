//
// Created by alireza on 25/05/22.
//

#ifndef DISCOT_SRC_MODEL_MODEL_H_
#define DISCOT_SRC_MODEL_MODEL_H_

#include "IObjective.h"
#include "SparsityConstraint.h"
#include "../../DOSE/include/Types.h"

namespace Scot {
class Model {
 public:
  Model(int number_of_nodes, int rank, double big_m_param);

  void setObjectiveFunction(IObjectivePtr new_objective_function);
  IObjectivePtr getObjectiveFunction();

  void setSparsityConstraint(SparsityConstraintPtr new_sparsity_constraint);
  SparsityConstraintPtr getSparsityConstraint();

  int getNumberOfVariables();
  int getNumberOfNonzeros();

  int getNumberOfNodes();
  double getBigMParam();

  int getRank();
  dose::ProblemType getProblemType();

 private:
  int number_of_variables_;
  int number_of_nonzeros_;
  int number_of_nodes_;
  int rank_;
  double big_m_param_;

  IObjectivePtr objective_function_;
  SparsityConstraintPtr sparsity_constraint_;
};

using ModelPtr = std::shared_ptr<Model>;
}
#endif //DISCOT_SRC_MODEL_MODEL_H_
