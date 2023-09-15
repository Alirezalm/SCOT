//
// Created by alireza on 30/05/22.
//

#ifndef DISCOT_SRC_ALGORITHM_DIPOA_H_
#define DISCOT_SRC_ALGORITHM_DIPOA_H_

#include "IAlgorithm.h"
#include "../PrimalSolver/InlpSolver.h"

namespace Scot {
/**
 * @brief This class implements the DIPOA algorithm
 */
class Dipoa : public IAlgorithm {

 public:
  /**
   * @brief default constructor
   * @param env solver environment
   */
  explicit Dipoa(EnvironmentPtr env);

  ~Dipoa() override = default;

  /**
   * @brief run the algorithm
   * @return true if the algorithm is successful
   */
  bool Run() override;

  /**
   * @brief output the header of the iteration table
   */
  void outputIterHeader();


 private:
  InlpSolverPtr nlp_solver_;

};

}

#endif //DISCOT_SRC_ALGORITHM_DIPOA_H_
