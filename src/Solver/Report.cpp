//
// Created by alireza on 26/06/22.
//

#include "Report.h"
namespace scot {

Report::Report(EnvironmentPtr env) : env_(env) {}
//todo: print model
//todo: print settings

void Report::printSolverHeader() {
  fmt::print("\n\n―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――\n");
  fmt::print("Sparse Convex Optimization Toolkit (SCOT)\n\n");
  fmt::print("a distributed MINLP Solver based on MPI   ---   v0.1.0\n\n");
  fmt::print("Alireza Olama - Federal University of Santa Catarina (UFSC), Brazil\n\n");
  fmt::print("email: alireza.lm69@gmail.com\n");
  fmt::print("github: github.com/alirezalm\n");

}

void Report::printIterationHeader() {
  fmt::print("     Cut Info     |       Objective Bounds      |      Gap Info        |  Elapsed    \n");
  fmt::print(" FOC          SOC | Incumbent        dual bound | abs              rel |       \n");
}
void Report::printIterationInfo(int nfoc,
                                int nsoc,
                                double incumbent,
                                double dual,
                                double abs_gap,
                                double rel_gap,
                                double elapsed) {
//  fmt::print("     Cut Info     |   Objective Bounds    |   Iter Time    \n");
//  fmt::print(" FOC          SOC | Incumbent        dual | primal   dual  \n");
  fmt::print(" {0:3d}          {1:3d} | {2:6.4f}        {3:6.4f} | {4:3.4f}      {5:3.4f} | {6:3.4f} s \n",
             nfoc, nsoc, incumbent, dual, abs_gap, rel_gap, elapsed
  );
}
void Report::printProblemDetails() {

  fmt::print("\nProblem details:\n―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――\n");
  fmt::print(fmt::format("\tnumber of nodes: {}\n", env_->model_->getNumberOfNodes()));
  switch (env_->model_->getProblemType()) {
    case dose::LinearRegression: fmt::print(fmt::format("\tproblem type: {}\n", "Sparse Regression"));
      break;
    case dose::LogisticRegression:fmt::print(fmt::format("\tproblem type: {}\n", "Sparse Classification"));
      break;
  }
  fmt::print(fmt::format("\tnumber of variables: {}\n", env_->model_->getNumberOfVariables()));
  fmt::print(fmt::format("\tnumber of rows per node: {}\n",
                         env_->model_->getObjectiveFunction()->getNumberOfRows()));
  fmt::print(fmt::format("\ttotal number of rows: {}\n",
                         env_->model_->getNumberOfNodes() * env_->model_->getObjectiveFunction()->getNumberOfRows()));

  fmt::print(fmt::format("\tnumber of nonzeros: {}\n", env_->model_->getNumberOfNonzeros()));

  fmt::print(fmt::format("\tbigM parameter: {}\n", env_->model_->getBigMParam()));
  fmt::print("\n");

}
void Report::printSolverDetails() {
  fmt::print("\nSCOT settings:\n―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――\n");
  fmt::print(fmt::format("\tAlgorithm: {}\n", env_->settings_->getStrSetting("algorithm")));
  //todo: to be completed
  fmt::print("\nNLP solver settings:\n―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――\n");
  fmt::print("\tAlgorithm Type: Distributed\n");
  fmt::print("\tAlgorithm name: RHADMM\n");
  fmt::print("\tDistributed computing framework: Message Passing Interface (MPI)\n");
  fmt::print("\nMIP solver settings:\n―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――\n");
  fmt::print("\tSolver name: Gurobi\n");
  fmt::print("\n―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――\n");
}

}