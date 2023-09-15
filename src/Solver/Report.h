//
// Created by alireza on 26/06/22.
//

#ifndef DISCOT_SRC_SOLVER_REPORT_H_
#define DISCOT_SRC_SOLVER_REPORT_H_

#include "Environment.h"
#include "../../ExternLibs/fmt/include/core.h"

namespace Scot {

class Report {

 public:
  explicit Report(EnvironmentPtr env);
  ~Report() = default;

  void printSolverHeader();
  void printProblemDetails();
  void printSolverDetails();
  void printIterationHeader();
  void printIterationInfo(int nfoc, int nsoc, double incumbent, double dual, double abs_gap, double rel_gap,
                          double elapsed);

 private:
  EnvironmentPtr env_;
};

}

#endif //DISCOT_SRC_SOLVER_REPORT_H_
