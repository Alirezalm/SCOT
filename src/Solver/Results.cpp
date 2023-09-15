//
// Created by alireza on 02/06/22.
//

#include "Results.h"
#include "Logger.h"
#include "../ExternLibs/fmt/include/core.h"
namespace Scot {

Results::Results(EnvironmentPtr env) {
  env_ = env;

}
Results::~Results() {
  incumbent_solutions_.clear();
  dual_solutions_.clear();
  iterations_.clear();
}

void Results::addIncumbent(IncumbentSolution current_incumbent) {
  incumbent_solution_ = current_incumbent;
  incumbent_solutions_.push_back(incumbent_solution_); // since primal solutions are feasible without constraints
  setBestPrimalBound(incumbent_solution_.total_obj_value);

  env_->logger_->logDebug("new incumbent solution added", env_->model_->getRank());

}
void Results::setBestPrimalBound(double value) {
  if (value >= best_primal_bound_) {
    env_->logger_->logDebug("primal objective bound not improved", env_->model_->getRank());
  }
  // we always store current best primal bound
  best_primal_bound_ = std::min(best_primal_bound_, value);
  best_primal_bounds_.push_back(best_primal_bound_);

}

double Results::getBestIncumbentBound() {
  return best_primal_bound_;
}
void Results::addDualSolution(DualSolution dual_solution) {
  dual_solution_ = dual_solution;
  dual_solutions_.push_back(dual_solution);
  env_->logger_->logDebug("new dual solution is added", env_->model_->getRank());
  setBestDualBound(dual_solution.objective_value);

}
void Results::setBestDualBound(double value) {
  if (value < best_dual_bound_) {
    env_->logger_->logWarning(
        fmt::format("dual objective bound is not strictly increasing with gap {}.", best_dual_bound_ - value),
        env_->model_->getRank()
        );
  }
  best_dual_bound_ = value;
  best_dual_bounds_.push_back(best_dual_bound_);
}

double Results::getBestDualBound() {
  return best_dual_bound_;
}

void Results::makeIteration() {
  iterations_.push_back(std::make_shared<Iteration>(env_));
}

IterationPtr Results::getCurrentIteration() {
  return iterations_.back();
}

int Results::getNumberOfIterations() {
  return static_cast<int>(iterations_.size());
}
IncumbentSolution Results::getCurrentIncumbent() {
  return incumbent_solution_;
}
DualSolution Results::getCurrentDualSolution() {
  return dual_solution_;
}
double Results::getAbsoluteOptimalityGap() {
  return best_abs_gap_;
}
void Results::setAbsoluteOptimalityGap(double abs_gap) {
  best_abs_gap_pre_ = best_abs_gap_;
  best_abs_gap_ = abs_gap;
}
double Results::getRelativeOptimalityGap() {
  return best_rel_gap_;
}
void Results::setRelativeOptimalityGap(double rel_gap) {
  best_rel_gap_pre_ = best_rel_gap_;
  best_rel_gap_ = rel_gap;
}

}