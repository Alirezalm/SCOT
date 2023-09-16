//
// Created by alireza on 30/05/22.
//
#include "ScotSolver.h"
#include "../Algorithm/Dihoa.h"
#include "../Algorithm/Dipoa.h"
#include "../ExternLibs/json/json.hpp"
#include "../MIPSolver/MipSolverGurobiMultipleTree.h"
#include "../Model/LinRegObjectiveFunction.h"
#include "../Model/LogRegObjectiveFunction.h"
#include "../Tasks/TaskQueue.h"
#include "Logger.h"
#include "MessagePassingInterface.h"
#include "Report.h"
#include "Results.h"
#include "Timer.h"
#include "fstream"

namespace Scot {
ScotSolver::ScotSolver() {

  Env = std::make_shared<Environment>();

  Env->TaskQueue = std::make_shared<TaskQueue>(Env);

  Env->Results = std::make_shared<Results>(Env);

  Env->Logger = std::make_shared<Logger>();

  Env->Report = std::make_shared<Report>(Env);

  Env->Settings = std::make_shared<Settings>();

  Env->Timer = std::make_shared<Timer>("scot_timer");
}

EnvironmentPtr ScotSolver::getEnvironment() { return Env; }

bool ScotSolver::solve() {
  if (Env->Settings->getDblSetting("verbose") == 1) {
    outputHeader();
  }
  selectAlgorithm();
  Env->Timer->start();
  bool isSolved = SolutionAlgorithm->run();

  if (Env->Model->getRank() == 1) {

    Env->Report->printOutputReport(isSolved);

    VectorDouble x = Env->Results->getCurrentIncumbent().consensus_solution;

    sparsify(x);
    saveResults(x);
    return isSolved;
  }
}
void ScotSolver::sparsify(VectorDouble &x) const {
  for (auto &item : x) {
    if (abs(item) <= 1e-5) {
      item = 0.0;
    }
  }
}
void ScotSolver::saveResults(VectorDouble &x) {
  nlohmann::json res;
  nlohmann::json j(x);
  res["objval"] = Env->Results->getCurrentIncumbent().total_obj_value;
  res["time"] = Env->Timer->elapsed();
  res["x"] = j;
  auto msg = fmt::format("rank_{0}_output.json", Env->Model->getRank());
  std::ofstream out(msg);
  out << std::setw(4) << res;
  out.close();
}

void ScotSolver::selectAlgorithm() {

  auto alg = Env->Settings->getStrSetting("algorithm");
  if (alg == "dipoa") {

    SolutionAlgorithm = std::make_shared<Dipoa>(Env);
    Env->Logger->logInfo("multi-tree algorithm, dipoa, selected.",
                         Env->Model->getRank());

  } else if (alg == "dihoa") {
    SolutionAlgorithm = std::make_shared<Dihoa>(Env);
    Env->Logger->logInfo("single-tree algorithm, dihoa, selected.",
                         Env->Model->getRank());
  } else {
    throw std::invalid_argument("no valid algorithm\n");
  }
}

void ScotSolver::outputHeader() {
  if (Env->Model->getRank() == 0) {
    Env->Report->printSolverHeader();
    Env->Report->printProblemDetails();
    Env->Report->printSolverDetails();
  }
}

bool ScotSolver::setProblemData(std::filesystem::path &inputPath) {

  std::ifstream input(inputPath);

  nlohmann::json problemJsonFormat;

  input >> problemJsonFormat;
  try {

    problemJsonFormat.at("response").get_to(Response);
    problemJsonFormat.at("samples").get_to(Samples);
    problemJsonFormat.at("type").get_to(Ptype);
  } catch (std::exception &exception) {

    throw std::invalid_argument("invalid problem data");
  }

  if (Ptype == "classification") {
    ProblemType = ProbType::CLASSIFICATION;
  } else if (Ptype == "regression") {
    ProblemType = ProbType::REGRESSION;
  } else {
    throw std::invalid_argument("unknown problem in solver.\n");
  }
  Upperbound = Env->Settings->getDblSetting("variable_bound");
  Env->Model = std::make_shared<Model>(TotalNodes, Rank, Upperbound);
  return false;
}

bool ScotSolver::setObjective() {
  std::shared_ptr<IObjective> obj;

  switch (ProblemType) {
  case ProbType::CLASSIFICATION:
    obj = std::make_shared<LogRegObjectiveFunction>();
    break;
  case ProbType::REGRESSION:
    obj = std::make_shared<LinRegObjectiveFunction>();
    break;
  default:
    throw std::invalid_argument("Error: Unsupported problem type.");
  }

  obj->setObjectiveData(Samples, Response);
  Env->Model->setObjectiveFunction(obj);
  return true;
}

bool ScotSolver::setSparseConstraints() {
  auto sparsityConstraint =
      std::make_shared<SparsityConstraint>(NumberOfNonzeros, Upperbound);
  Env->Model->setSparsityConstraint(sparsityConstraint);
  return true;
}

bool ScotSolver::setMpi() {
  Env->Mpi = std::make_shared<MessagePassingInterface>(Env);
  return true;
}

void ScotSolver::setTotalNodes(int totalNodes) {
  if (totalNodes < 1) {
    throw std::invalid_argument(
        "Error: number of nodes must be positive integer");
  }
  TotalNodes = totalNodes;
}

void ScotSolver::setRank(int rank) {
  if (rank < 0) {
    throw std::invalid_argument("Error: MPI rank cannot be negative");
  }
  Rank = rank;
}

bool ScotSolver::setNumberOfNonzeros(int nzeros) {
  if (nzeros < 1) {
    throw std::invalid_argument(
        "SCOT error: number of non-zeros must be positive.\n");
  }
  int totalNumberOfVariables = Env->Model->getNumberOfVariables();

  if (nzeros >= totalNumberOfVariables) {
    throw std::invalid_argument(
        "SCOT error: number of non-zeros cannot be larger than variables.\n");
  }
  NumberOfNonzeros = nzeros;
  return true;
}

} // namespace Scot