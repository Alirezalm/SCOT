//
// Created by alireza on 30/05/22.
//

#ifndef DISCOT_SRC_ALGORITHM_DIPOA_H_
#define DISCOT_SRC_ALGORITHM_DIPOA_H_

#include "IAlgorithm.h"
#include "../PrimalSolver/InlpSolver.h"

namespace scot {

class Dipoa : public IAlgorithm {
 public:

  explicit Dipoa(EnvironmentPtr env);
  ~Dipoa() override = default;

  bool Run() override;
  void outputIterHeader();
 private:
  InlpSolverPtr nlp_solver_;

};

}

#endif //DISCOT_SRC_ALGORITHM_DIPOA_H_
