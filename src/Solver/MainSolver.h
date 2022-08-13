//
// Created by alireza on 30/05/22.
//

#ifndef SCOT_SRC_SOLVER_MAINSOLVER_H_
#define SCOT_SRC_SOLVER_MAINSOLVER_H_
#include "filesystem"
#include "Environment.h"
#include "../Algorithm/IAlgorithm.h"
#include "Enums.h"
namespace scot {
class MainSolver {

 public:
  //todo: solver can receive no argument and construct the environment **IMPORTANT**
  explicit MainSolver();
  ~MainSolver() = default;
  void outputHeader();

  bool setProblemData(std::filesystem::path input_path);
  bool setNumberOfNonzeros(int nzeros);
  bool setObjective();
  bool setSparseConstraints();
  bool setMpi();

  void setTotalNodes(int total_nodes);
  void setRank(int rank);


  bool solve();

  EnvironmentPtr getEnvironment();

 private:
  // methods
  void selectAlgorithm();
  // data members
  EnvironmentPtr env_;
  IAlgorithmPtr solution_algorithm_;
  int nnzeros_;
  int total_nodes_;
  int rank_;
  double upperbound_;
  VectorDouble response_;
  VectorDouble2D samples_;
  ProbType problem_type_;
  std::string ptype_;
};
}

#endif //SCOT_SRC_SOLVER_MAINSOLVER_H_
