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

  Env->task_queue_ptr_ = std::make_shared<TaskQueue>(Env);

  Env->results_ = std::make_shared<Results>(Env);

  Env->logger_ = std::make_shared<Logger>();

  Env->report_ = std::make_shared<Report>(Env);

  Env->settings_ = std::make_shared<Settings>();

  Env->timer_ = std::make_shared<Timer>("scot_timer");
}

EnvironmentPtr ScotSolver::getEnvironment() { return Env; }

bool ScotSolver::solve() {
  if (Env->settings_->getDblSetting("verbose") == 1) {
    outputHeader();
  }
  selectAlgorithm();
  Env->timer_->start();
  return SolutionAlgorithm->run();
}

void ScotSolver::selectAlgorithm() {

  auto alg = Env->settings_->getStrSetting("algorithm");
  if (alg == "dipoa") {

    SolutionAlgorithm = std::make_shared<Dipoa>(Env);
    Env->logger_->logInfo("multi-tree algorithm, dipoa, selected.",
                          Env->model_->getRank());

  } else if (alg == "dihoa") {
    SolutionAlgorithm = std::make_shared<Dihoa>(Env);
    Env->logger_->logInfo("single-tree algorithm, dihoa, selected.",
                          Env->model_->getRank());
  } else {
    //    env_->logger_->logCritical("no algorithm provided, dipoa is selected",
    //    env_->model_->getRank());
    throw std::invalid_argument("no valid algorithm\n");
  }
}

void ScotSolver::outputHeader() {
  if (Env->model_->getRank() == 0) {
    Env->report_->printSolverHeader();
    Env->report_->printProblemDetails();
    Env->report_->printSolverDetails();
  }
}

bool ScotSolver::setProblemData(std::filesystem::path inputPath) {

  std::ifstream input(inputPath);

  nlohmann::json problem_json;

  input >> problem_json;

  problem_json.at("response").get_to(Response);
  problem_json.at("samples").get_to(Samples);
  problem_json.at("type").get_to(Ptype);
  if (Ptype == "classification") {
    ProblemType = ProbType::CLASSIFICATION;
  } else if (Ptype == "regression") {
    ProblemType = ProbType::REGRESSION;
  } else {
    throw std::invalid_argument("unknown problem in solver.\n");
  }
  Upperbound = 30; // todo: for now
  Env->model_ = std::make_shared<Model>(TotalNodes, Rank, Upperbound);
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
  }

  obj->setObjectiveData(Samples, Response);
  Env->model_->setObjectiveFunction(obj);
  return true;
}

bool ScotSolver::setSparseConstraints() {
  auto sparsity_constraint =
      std::make_shared<SparsityConstraint>(NumberOfNonzeros, Upperbound);
  Env->model_->setSparsityConstraint(sparsity_constraint);
  return true;
}

bool ScotSolver::setMpi() {
  Env->mpi_ = std::make_shared<MessagePassingInterface>(Env);
  return true;
}

void ScotSolver::setTotalNodes(int totalNodes) { TotalNodes = totalNodes; }

void ScotSolver::setRank(int rank) { Rank = rank; }

bool ScotSolver::setNumberOfNonzeros(int nzeros) {
  // todo: perform checks
  if (nzeros < 1) {
    throw std::invalid_argument(
        "SCOT error: number of nonzeros must be positive.\n");
  }
  if (nzeros >= Env->model_->getNumberOfVariables()) {
    throw std::invalid_argument(
        "SCOT error: number of nonzeros cannot be larger than variables.\n");
  }
  NumberOfNonzeros = nzeros;
  return true;
}

} // namespace Scot