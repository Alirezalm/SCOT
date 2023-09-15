//
// Created by alireza on 26/06/22.
//

#include "Report.h"
#include "Results.h"
#include "Timer.h"
namespace Scot {

Report::Report(EnvironmentPtr env) : env_(env) {}
// todo: print model
// todo: print settings

void Report::printSolverHeader() {
  fmt::print("\n\n―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
             "――――――――――――――\n");
  fmt::print("Sparse Convex Optimization Toolkit (SCOT)\n\n");
  fmt::print("a distributed MINLP Solver based on MPI   ---   v0.1.0\n\n");
  fmt::print("Alireza Olama - Federal University of Santa Catarina (UFSC), "
             "Brazil\n\n");
  fmt::print("email: alireza.lm69@gmail.com\n");
  fmt::print("github: github.com/alirezalm/scot\n");
}

void Report::printIterationHeader() {
  fmt::print("       Cut Info        |     Objective Bounds      |      Gap "
             "Info  |  Elapsed    \n");
  fmt::print("   FOC             SOC | Incumbent      dual | abs              "
             "rel |       \n");
}
void Report::printIterationInfo(int nfoc, int nsoc, double incumbent,
                                double dual, double absGap, double relGap,
                                double elapsed) {
  //  fmt::print("     Cut Info     |   Objective Bounds    |   Iter Time \n");
  //  fmt::print(" FOC          SOC | Incumbent        dual | primal   dual
  //  \n");
  fmt::print(" {0:5d}          {1:5d} | {2:6.4f}        {3:6.4f} | {4:3.4f}    "
             "  {5:3.4f} | {6:3.4f} s \n",
             nfoc, nsoc, incumbent, dual, absGap, relGap, elapsed);
}
void Report::printProblemDetails() {

  fmt::print("\nProblem details:\n"
             "―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
             "――――――――――\n"
             "\tnumber of nodes: {}\n",
             env_->Model->getNumberOfNodes());

  std::string problemTypeStr;
  switch (env_->Model->getProblemType()) {
  case dose::LINEAR_REGRESSION:
    problemTypeStr = "Sparse Regression";
    break;
  case dose::LOGISTIC_REGRESSION:
    problemTypeStr = "Sparse Classification";
    break;
  }
  fmt::print("\tproblem type: {}\n", problemTypeStr);

  fmt::print("\tnumber of variables: {}\n"
             "\tnumber of rows per node: {}\n"
             "\ttotal number of rows: {}\n"
             "\tnumber of nonzeros: {}\n"
             "\tbigM parameter: {}\n\n",
             env_->Model->getNumberOfVariables(),
             env_->Model->getObjectiveFunction()->getNumberOfRows(),
             env_->Model->getNumberOfNodes() *
                 env_->Model->getObjectiveFunction()->getNumberOfRows(),
             env_->Model->getNumberOfNonzeros(), env_->Model->getBigMParam());
}
void Report::printSolverDetails() {
  fmt::print("\nSCOT settings:\n"
             "―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
             "―――――――――\n"
             "\tAlgorithm: {}\n",
             env_->Settings->getStrSetting("algorithm"));

  fmt::print(
      "\nNLP solver settings:\n"
      "――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
      "――\n"
      "\tAlgorithm Type: Distributed\n"
      "\tAlgorithm name: RHADMM\n"
      "\tDistributed computing framework: Message Passing Interface (MPI)\n");

  fmt::print("\nMIP solver settings:\n"
             "―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
             "―――――――――\n"
             "\tSolver name: Gurobi\n");

  fmt::print("\n―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
             "―――――――――――\n");
}
void Report::printOutputReport(bool isSolved) {
  fmt::print(
      "\nSolution Report:\n"
      "――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――"
      "――\n");
  if (!isSolved) {
    fmt::print("SCOT did not converge successfully");
  }

  fmt::print("\tSCOT converged successfully\n");

  fmt::print("\tOptimal objective value: {0:5.4f}\n",
             env_->Results->getBestIncumbentBound());
  fmt::print("\tExecution time: {0:5.4f} seconds\n", env_->Timer->elapsed());
}

} // namespace Scot