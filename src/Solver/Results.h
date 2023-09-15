//
// Created by alireza on 02/06/22.
//

#ifndef DISCOT_SRC_SOLVER_RESULTS_H_
#define DISCOT_SRC_SOLVER_RESULTS_H_

#include "Environment.h"
#include "Structs.h"
#include "Iteration.h"

namespace Scot {

class Results {

 public:
  explicit Results(EnvironmentPtr env);
  ~Results(); // clear the vectors

  void addIncumbent(IncumbentSolution current_incumbent);
  void addDualSolution(DualSolution dual_solution);

  IncumbentSolution getCurrentIncumbent();
  DualSolution getCurrentDualSolution();

  double getBestIncumbentBound();
  double getBestDualBound();


//todo: methods related to termination must be implemented

//  bool isRelativeOptimalityGapMet();
//
//  bool isAbsoluteOptimalityGapMet();

  double getRelativeOptimalityGap();
  void setRelativeOptimalityGap(double rel_gap);
//
  double getAbsoluteOptimalityGap();
  void setAbsoluteOptimalityGap(double abs_gap);

  void makeIteration();

  std::vector<IterationPtr> iterations_;
  IterationPtr getCurrentIteration();
  int getNumberOfIterations();

  //Todo: maybe enum of MIP solvers?
  //TODO: file operations (e.g. saving results to json etc)

  double best_abs_gap_pre_;
  double best_rel_gap_pre_;
 private:
  void setBestPrimalBound(double value);
  void setBestDualBound(double value);

  IncumbentSolution incumbent_solution_;
  std::vector<IncumbentSolution> incumbent_solutions_;

  DualSolution dual_solution_;
  std::vector<DualSolution> dual_solutions_;

  VectorDouble best_dual_bounds_;
  VectorDouble best_primal_bounds_;

  double best_abs_gap_;
  double best_rel_gap_;


  EnvironmentPtr env_;

  //todo: must move to constants
  double best_dual_bound_ = -1e10;
  double best_primal_bound_ = 1e10;
};

}

#endif //DISCOT_SRC_SOLVER_RESULTS_H_
