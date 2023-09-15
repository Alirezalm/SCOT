//
// Created by alireza on 30/05/22.
//

#ifndef DISCOT_SRC_ALGORITHM_IALGORITHM_H_
#define DISCOT_SRC_ALGORITHM_IALGORITHM_H_
#include "../Solver/Environment.h"
namespace Scot {
/**
 * @brief This class is the base class for all scot algorithms
 */
class IAlgorithm {
 public:

  virtual ~IAlgorithm() = default;

  /**
   * @brief run the algorithm
   * @return true if the algorithm is successful
   */
  virtual bool Run() = 0;

 protected:
  EnvironmentPtr env_;
};

using IAlgorithmPtr = std::shared_ptr<IAlgorithm>;
}
#endif //DISCOT_SRC_ALGORITHM_IALGORITHM_H_
