//
// Created by alireza on 20/06/22.
//

#include "MessagePassingInterface.h"
#include "include/Exceptions.h"
#include "../Solver/Logger.h"

namespace Scot {

const int kRootNode = 0;

MessagePassingInterface::MessagePassingInterface(EnvironmentPtr env) {
  env_ = env;
  mpi_gather_rcv_x_.resize(env_->Model->getNumberOfVariables() * env->Model->getNumberOfNodes());
  mpi_gather_rcv_gx_.resize(env_->Model->getNumberOfVariables() * env->Model->getNumberOfNodes());
  mpi_gather_rcv_obj_.resize(env_->Model->getNumberOfNodes());

  linear_outer_approximations_.resize(env_->Model->getNumberOfNodes());
  mpi_gather_rcv_min_eig_.resize(env_->Model->getNumberOfNodes());
}
void MessagePassingInterface::gather(LinearOuterApproximation linear_outer_approximation) {
  try {
    MPI_Allgather(
        linear_outer_approximation.consensus_feasible_point.data(),
        static_cast<int>(linear_outer_approximation.consensus_feasible_point.size()),
        MPI_DOUBLE,
        mpi_gather_rcv_x_.data(),
        static_cast<int>(linear_outer_approximation.consensus_feasible_point.size()),
        MPI_DOUBLE,
        MPI_COMM_WORLD
    );

    MPI_Allgather(
        linear_outer_approximation.local_gradient_at_feasible_point.data(),
        static_cast<int>(linear_outer_approximation.local_gradient_at_feasible_point.size()),
        MPI_DOUBLE,
        mpi_gather_rcv_gx_.data(),
        static_cast<int>(linear_outer_approximation.local_gradient_at_feasible_point.size()),
        MPI_DOUBLE,
        MPI_COMM_WORLD
    );

    MPI_Allgather(
        &linear_outer_approximation.local_objective_value_at_feasible_point,
        1,
        MPI_DOUBLE,
        mpi_gather_rcv_obj_.data(),
        1,
        MPI_DOUBLE,
        MPI_COMM_WORLD
    );
  } catch (std::exception &exception) {

    env_->Logger->logCritical(exception.what(), env_->Model->getRank());

  }

}
void MessagePassingInterface::gather(QuadraticOuterApproximation quadratic_outer_approximation) {

  try {
    MPI_Allgather(
        quadratic_outer_approximation.consensus_feasible_point.data(),
        static_cast<int>(quadratic_outer_approximation.consensus_feasible_point.size()),
        MPI_DOUBLE,
        mpi_gather_rcv_x_.data(),
        static_cast<int>(quadratic_outer_approximation.consensus_feasible_point.size()),
        MPI_DOUBLE,
        MPI_COMM_WORLD
    );
    MPI_Allgather(
        quadratic_outer_approximation.local_gradient_at_feasible_point.data(),
        static_cast<int>(quadratic_outer_approximation.local_gradient_at_feasible_point.size()),
        MPI_DOUBLE,
        mpi_gather_rcv_gx_.data(),
        static_cast<int>(quadratic_outer_approximation.local_gradient_at_feasible_point.size()),
        MPI_DOUBLE,
        MPI_COMM_WORLD
    );
    MPI_Allgather(
        &quadratic_outer_approximation.local_objective_value_at_feasible_point,
        1,
        MPI_DOUBLE,
        mpi_gather_rcv_obj_.data(),
        1,
        MPI_DOUBLE,
        MPI_COMM_WORLD
    );

    MPI_Allgather(
        &quadratic_outer_approximation.local_min_eig_at_feasible_point,
        1,
        MPI_DOUBLE,
        mpi_gather_rcv_min_eig_.data(),
        1,
        MPI_DOUBLE,
        MPI_COMM_WORLD
    );

  } catch (std::exception &exception) {

    env_->Logger->logCritical(exception.what(), env_->Model->getRank());

  }

}
LinearOuterApproximation MessagePassingInterface::getLinearOuterApproximation(int node_index) {
  //todo: make sure gather is run before

  auto begin = node_index * env_->Model->getNumberOfVariables();

  auto end = begin + env_->Model->getNumberOfVariables();

  auto local_x = std::vector<double>(mpi_gather_rcv_x_.begin() + begin, mpi_gather_rcv_x_.begin() + end);
  auto local_gx = std::vector<double>(mpi_gather_rcv_gx_.begin() + begin, mpi_gather_rcv_gx_.begin() + end);

  auto local_obj = mpi_gather_rcv_obj_.at(node_index);

  auto local_oa_cut = LinearOuterApproximation();
  local_oa_cut.consensus_feasible_point = local_x;
  local_oa_cut.local_gradient_at_feasible_point = local_gx;
  local_oa_cut.local_objective_value_at_feasible_point = local_obj;

  return local_oa_cut;
}
QuadraticOuterApproximation MessagePassingInterface::getQuadraticOuterApproximation(int node_index) {
  auto begin = node_index * env_->Model->getNumberOfVariables();

  auto end = begin + env_->Model->getNumberOfVariables();

  auto local_x = std::vector<double>(mpi_gather_rcv_x_.begin() + begin, mpi_gather_rcv_x_.begin() + end);
  auto local_gx = std::vector<double>(mpi_gather_rcv_gx_.begin() + begin, mpi_gather_rcv_gx_.begin() + end);

  auto local_obj = mpi_gather_rcv_obj_.at(node_index);
  auto local_min_eig = mpi_gather_rcv_min_eig_.at(node_index);

  auto local_oa_cut = QuadraticOuterApproximation();
  local_oa_cut.consensus_feasible_point = local_x;
  local_oa_cut.local_gradient_at_feasible_point = local_gx;
  local_oa_cut.local_objective_value_at_feasible_point = local_obj;
  local_oa_cut.local_min_eig_at_feasible_point = local_min_eig;
  return local_oa_cut;
}
DualSolution MessagePassingInterface::bcast(DualSolution dual_solution) {

  dual_solution.real_vector.resize(env_->Model->getNumberOfVariables());
  dual_solution.binary_vector.resize(env_->Model->getNumberOfVariables());

  MPI_Bcast(
      dual_solution.binary_vector.data(),
      static_cast<int>(dual_solution.binary_vector.size()),
      MPI_DOUBLE,
      kRootNode,
      MPI_COMM_WORLD
  );
  try {
    MPI_Bcast(
        dual_solution.real_vector.data(),
        static_cast<int>(dual_solution.real_vector.size()),
        MPI_DOUBLE,
        kRootNode,
        MPI_COMM_WORLD
    );
    MPI_Bcast(
        &dual_solution.objective_value,
        1,
        MPI_DOUBLE,
        kRootNode,
        MPI_COMM_WORLD
    );
  } catch (std::exception &exception) {

    env_->Logger->logCritical(exception.what(), env_->Model->getRank());

  }

  return dual_solution;

}

}