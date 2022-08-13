//
// Created by alireza on 02/06/22.
//

#ifndef DISCOT_SRC_ITERATION_H_
#define DISCOT_SRC_ITERATION_H_
//todo: update iteration info after each iteration
#include "Environment.h"

namespace scot {

class Iteration {

 public:
  explicit Iteration(EnvironmentPtr env);
  ~Iteration() = default;

  int iteration_number_ = 0;
  int number_first_order_oa_cuts_ = 0;

  int number_second_order_oa_cuts_ = 0;
  int number_ecp_cuts_ = 0;

  int total_number_of_cuts_ = 0;
  bool is_solved_ = false;
  double iteration_time_; //todo: important

 private:
  EnvironmentPtr env_;
};

using IterationPtr = std::shared_ptr<Iteration>;

}

#endif //DISCOT_SRC_ITERATION_H_
