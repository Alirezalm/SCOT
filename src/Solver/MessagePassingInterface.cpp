//
// Created by alireza on 20/06/22.
//

#include "MessagePassingInterface.h"
#include "../Solver/Logger.h"
#include "include/Exceptions.h"

namespace Scot {

const int kRootNode = 0;

MessagePassingInterface::MessagePassingInterface(EnvironmentPtr env) {
  Env = env;
  MpiGatherRcvX.resize(Env->Model->getNumberOfVariables() *
                           env->Model->getNumberOfNodes());
  MpiGatherRcvGx.resize(Env->Model->getNumberOfVariables() *
                            env->Model->getNumberOfNodes());
  MpiGatherRcvObj.resize(Env->Model->getNumberOfNodes());

  LinearOuterApproximations.resize(Env->Model->getNumberOfNodes());
  MpiGatherRcvMinEig.resize(Env->Model->getNumberOfNodes());
}
void MessagePassingInterface::gather(
    LinearOuterApproximation linearOuterApproximation) {
  try {
    MPI_Allgather(linearOuterApproximation.consensus_feasible_point.data(),
        static_cast<int>(
                      linearOuterApproximation.consensus_feasible_point.size()),
        MPI_DOUBLE, MpiGatherRcvX.data(),
        static_cast<int>(
                      linearOuterApproximation.consensus_feasible_point.size()),
        MPI_DOUBLE, MPI_COMM_WORLD);

    MPI_Allgather(
        linearOuterApproximation.local_gradient_at_feasible_point.data(),
        static_cast<int>(
            linearOuterApproximation.local_gradient_at_feasible_point.size()),
        MPI_DOUBLE, MpiGatherRcvGx.data(),
        static_cast<int>(
            linearOuterApproximation.local_gradient_at_feasible_point.size()),
        MPI_DOUBLE, MPI_COMM_WORLD);

    MPI_Allgather(
        &linearOuterApproximation.local_objective_value_at_feasible_point, 1,
        MPI_DOUBLE, MpiGatherRcvObj.data(), 1, MPI_DOUBLE, MPI_COMM_WORLD);
  } catch (std::exception &exception) {

    Env->Logger->logCritical(exception.what(), Env->Model->getRank());
  }
}
void MessagePassingInterface::gather(
    QuadraticOuterApproximation quadraticOuterApproximation) {

  try {
    MPI_Allgather(
        quadraticOuterApproximation.consensus_feasible_point.data(),
        static_cast<int>(
            quadraticOuterApproximation.consensus_feasible_point.size()),
        MPI_DOUBLE, MpiGatherRcvX.data(),
        static_cast<int>(
            quadraticOuterApproximation.consensus_feasible_point.size()),
        MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Allgather(
        quadraticOuterApproximation.local_gradient_at_feasible_point.data(),
        static_cast<int>(quadraticOuterApproximation
                             .local_gradient_at_feasible_point.size()),
        MPI_DOUBLE, MpiGatherRcvGx.data(),
        static_cast<int>(quadraticOuterApproximation
                             .local_gradient_at_feasible_point.size()),
        MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Allgather(
        &quadraticOuterApproximation.local_objective_value_at_feasible_point,
        1, MPI_DOUBLE, MpiGatherRcvObj.data(), 1, MPI_DOUBLE,
        MPI_COMM_WORLD);

    MPI_Allgather(
        &quadraticOuterApproximation.local_min_eig_at_feasible_point, 1,
        MPI_DOUBLE, MpiGatherRcvMinEig.data(), 1, MPI_DOUBLE,
        MPI_COMM_WORLD);

  } catch (std::exception &exception) {

    Env->Logger->logCritical(exception.what(), Env->Model->getRank());
  }
}
LinearOuterApproximation
MessagePassingInterface::getLinearOuterApproximation(int nodeIndex) {
  // todo: make sure gather is run before

  auto begin = nodeIndex * Env->Model->getNumberOfVariables();

  auto end = begin + Env->Model->getNumberOfVariables();

  auto localX = std::vector<double>(MpiGatherRcvX.begin() + begin,
                                     MpiGatherRcvX.begin() + end);
  auto localGx = std::vector<double>(MpiGatherRcvGx.begin() + begin,
                                      MpiGatherRcvGx.begin() + end);

  auto localObj = MpiGatherRcvObj.at(nodeIndex);

  auto localOaCut = LinearOuterApproximation();
  localOaCut.consensus_feasible_point = localX;
  localOaCut.local_gradient_at_feasible_point = localGx;
  localOaCut.local_objective_value_at_feasible_point = localObj;

  return localOaCut;
}
QuadraticOuterApproximation
MessagePassingInterface::getQuadraticOuterApproximation(int nodeIndex) {
  auto begin = nodeIndex * Env->Model->getNumberOfVariables();

  auto end = begin + Env->Model->getNumberOfVariables();

  auto localX = std::vector<double>(MpiGatherRcvX.begin() + begin,
                                     MpiGatherRcvX.begin() + end);
  auto localGx = std::vector<double>(MpiGatherRcvGx.begin() + begin,
                                      MpiGatherRcvGx.begin() + end);

  auto localObj = MpiGatherRcvObj.at(nodeIndex);
  auto localMinEig = MpiGatherRcvMinEig.at(nodeIndex);

  auto localOaCut = QuadraticOuterApproximation();
  localOaCut.consensus_feasible_point = localX;
  localOaCut.local_gradient_at_feasible_point = localGx;
  localOaCut.local_objective_value_at_feasible_point = localObj;
  localOaCut.local_min_eig_at_feasible_point = localMinEig;
  return localOaCut;
}
DualSolution MessagePassingInterface::bcast(DualSolution dualSolution) {

  dualSolution.real_vector.resize(Env->Model->getNumberOfVariables());
  dualSolution.binary_vector.resize(Env->Model->getNumberOfVariables());

  MPI_Bcast(dualSolution.binary_vector.data(),
            static_cast<int>(dualSolution.binary_vector.size()), MPI_DOUBLE,
            kRootNode, MPI_COMM_WORLD);
  try {
    MPI_Bcast(dualSolution.real_vector.data(),
              static_cast<int>(dualSolution.real_vector.size()), MPI_DOUBLE,
              kRootNode, MPI_COMM_WORLD);
    MPI_Bcast(&dualSolution.objective_value, 1, MPI_DOUBLE, kRootNode,
              MPI_COMM_WORLD);
  } catch (std::exception &exception) {

    Env->Logger->logCritical(exception.what(), Env->Model->getRank());
  }

  return dualSolution;
}

} // namespace Scot