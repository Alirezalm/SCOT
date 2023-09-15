//
// Created by alireza on 02/06/22.
//

#ifndef SCOT_SRC_SOLVER_RESULTS_H_
#define SCOT_SRC_SOLVER_RESULTS_H_

#include "Environment.h"
#include "Iteration.h"
#include "Structs.h"

namespace Scot {

class Results {

public:

  explicit Results(EnvironmentPtr env);
  ~Results(); // clear the vectors

  void addIncumbent(IncumbentSolution currentIncumbent);
  void addDualSolution(DualSolution dualSolution);

  IncumbentSolution getCurrentIncumbent();
  DualSolution getCurrentDualSolution();

  double getBestIncumbentBound();
  double getBestDualBound();

  // todo: methods related to termination must be implemented

  //  bool isRelativeOptimalityGapMet();
  //
  //  bool isAbsoluteOptimalityGapMet();

  double getRelativeOptimalityGap();
  void setRelativeOptimalityGap(double relGap);
  //
  double getAbsoluteOptimalityGap();
  void setAbsoluteOptimalityGap(double absGap);

  void makeIteration();

  std::vector<IterationPtr> Iterations;
  IterationPtr getCurrentIteration();
  int getNumberOfIterations();

  // Todo: maybe enum of MIP solvers?
  // TODO: file operations (e.g. saving results to json etc)

  double BestAbsGapPre;
  double BestRelGapPre;

private:
  void setBestPrimalBound(double value);
  void setBestDualBound(double value);

  IncumbentSolution CurrentIncumbentSolution;
  std::vector<IncumbentSolution> IncumbentSolutions;

  DualSolution CurrentDualSolution;
  std::vector<DualSolution> DualSolutions;

  VectorDouble BestDualBounds;
  VectorDouble BestPrimalBounds;

  double BestAbsGap;
  double BestRelGap;

  EnvironmentPtr Env;

  // todo: must move to constants
  double BestDualBound = -1e10;
  double BestPrimalBound = 1e10;
};

} // namespace Scot

#endif // SCOT_SRC_SOLVER_RESULTS_H_
