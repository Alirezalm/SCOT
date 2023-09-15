//
// Created by alireza on 02/06/22.
//

#include "Results.h"
#include "Logger.h"
#include "../ExternLibs/fmt/include/core.h"
namespace Scot {

Results::Results(EnvironmentPtr env) { Env = env;

}
Results::~Results() {
  IncumbentSolutions.clear();
  DualSolutions.clear();
  Iterations.clear();
}

void Results::addIncumbent(IncumbentSolution currentIncumbent) {
  CurrentIncumbentSolution = currentIncumbent;
  IncumbentSolutions.push_back(
      CurrentIncumbentSolution); // since primal solutions are feasible without constraints
  setBestPrimalBound(CurrentIncumbentSolution.total_obj_value);

  Env->Logger->logDebug("new incumbent solution added", Env->Model->getRank());

}
void Results::setBestPrimalBound(double value) {
  if (value >= BestPrimalBound) {
    Env->Logger->logDebug("primal objective bound not improved",
                          Env->Model->getRank());
  }
  // we always store current best primal bound
  BestPrimalBound = std::min(BestPrimalBound, value);
  BestPrimalBounds.push_back(BestPrimalBound);

}

double Results::getBestIncumbentBound() {
  return BestPrimalBound;
}
void Results::addDualSolution(DualSolution dualSolution) {
  CurrentDualSolution = dualSolution;
  DualSolutions.push_back(dualSolution);
  Env->Logger->logDebug("new dual solution is added", Env->Model->getRank());
  setBestDualBound(dualSolution.objective_value);

}
void Results::setBestDualBound(double value) {
  if (value < BestDualBound) {
    Env->Logger->logWarning(
        fmt::format("dual objective bound is not strictly increasing with gap {}.",
            BestDualBound - value),
        Env->Model->getRank()
        );
  }
  BestDualBound = value;
  BestDualBounds.push_back(BestDualBound);
}

double Results::getBestDualBound() {
  return BestDualBound;
}

void Results::makeIteration() {
  Iterations.push_back(std::make_shared<Iteration>(Env));
}

IterationPtr Results::getCurrentIteration() {
  return Iterations.back();
}

int Results::getNumberOfIterations() {
  return static_cast<int>(Iterations.size());
}
IncumbentSolution Results::getCurrentIncumbent() {
  return CurrentIncumbentSolution;
}
DualSolution Results::getCurrentDualSolution() {
  return CurrentDualSolution;
}
double Results::getAbsoluteOptimalityGap() {
  return BestAbsGap;
}
void Results::setAbsoluteOptimalityGap(double absGap) {
  BestAbsGapPre = BestAbsGap;
  BestAbsGap = absGap;
}
double Results::getRelativeOptimalityGap() {
  return BestRelGap;
}
void Results::setRelativeOptimalityGap(double relGap) {
  BestRelGapPre = BestRelGap;
  BestRelGap = relGap;
}

}