//
// Created by alireza on 20/06/22.
//

#ifndef DISCOT_SRC_MESSAGEPASSINGINTERFACE_H_
#define DISCOT_SRC_MESSAGEPASSINGINTERFACE_H_
//todo: voids -> bool and error check
#include "Environment.h"

namespace scot {
class MessagePassingInterface {

 public:
  explicit MessagePassingInterface(EnvironmentPtr env);
  ~MessagePassingInterface() = default;

  void gather(LinearOuterApproximation linear_outer_approximation);
  void gather(QuadraticOuterApproximation quadratic_outer_approximation);

  DualSolution bcast(DualSolution dual_solution);
  LinearOuterApproximation getLinearOuterApproximation(int node_index);
  QuadraticOuterApproximation getQuadraticOuterApproximation(int node_index);
 private:

  EnvironmentPtr env_;
  VectorDouble mpi_gather_rcv_x_;
  VectorDouble mpi_gather_rcv_gx_;
  VectorDouble mpi_gather_rcv_obj_;
  VectorDouble mpi_gather_rcv_min_eig_;
  std::vector<LinearOuterApproximation> linear_outer_approximations_;
};
}

#endif //DISCOT_SRC_MESSAGEPASSINGINTERFACE_H_
