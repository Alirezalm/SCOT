//
// Created by alireza on 26/06/22.
//

#ifndef DISCOT_SRC_SOLVER_REPORT_H_
#define DISCOT_SRC_SOLVER_REPORT_H_

#include "../../ExternLibs/fmt/include/core.h"
#include "Environment.h"

namespace Scot {

class Report {

public:
  explicit Report(EnvironmentPtr env);
  ~Report() = default;

  void printSolverHeader();
  void printProblemDetails();
  void printSolverDetails();
  void printIterationHeader();
  void printIterationInfo(int nfoc, int nsoc, double incumbent, double dual,
                          double absGap, double relGap, double elapsed);
  void printOutputReport(bool isSolved);

private:
  EnvironmentPtr env_;
};

} // namespace Scot

#endif // DISCOT_SRC_SOLVER_REPORT_H_
