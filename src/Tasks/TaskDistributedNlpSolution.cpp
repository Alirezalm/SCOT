//
// Created by alireza on 03/06/22.
//

#include "TaskDistributedNlpSolution.h"
#include "../Solver/Results.h"
#include "../Solver/Logger.h"

#include "../PrimalSolver/DistributedOperatorSplittingEngine.h"

namespace scot {

TaskDistributedNlpSolution::TaskDistributedNlpSolution(EnvironmentPtr env) : TaskBase(env) {}

void TaskDistributedNlpSolution::execute() {
// todo: corner cases
//todo: read primal settings

  createNlpSolver();
  initialize();
  auto nlp_solution = getNlpSolution();
  env_->results_->addIncumbent(nlp_solution);
}

void TaskDistributedNlpSolution::createNlpSolver() {
// todo: for now it is distributed solver. have to write factory if other primal solvers are available
  nlp_solver_ = std::make_shared<DistributedOperatorSplittingEngine>(env_);

}

void TaskDistributedNlpSolution::initialize() {

  if (env_->results_->getCurrentIteration()->iteration_number_==1) {

	auto current_dual_solution = std::vector<double>(env_->model_->getNumberOfVariables(), 0);

	nlp_solver_->setFixedBinaryVariables(current_dual_solution);

	//todo: feasiblity pump applies here
	env_->logger_->logDebug("initial binary combination set to zero", env_->model_->getRank());

  } else {
	auto current_dual_solution = env_->results_->getCurrentDualSolution();

	nlp_solver_->setFixedBinaryVariables(current_dual_solution.binary_vector);

	env_->logger_->logDebug("querying new binary combination for nlp solver", env_->model_->getRank());

  }

}

IncumbentSolution TaskDistributedNlpSolution::getNlpSolution() {
  nlp_solver_->solveProblem();
  return nlp_solver_->getPrimalSolution();
}

}