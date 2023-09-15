//
// Created by alireza on 25/07/22.
//

#ifndef SCOT_SRC_SOLVER_TIMER_H_
#define SCOT_SRC_SOLVER_TIMER_H_
#include "string"
#include "chrono"

namespace Scot {

class Timer {
 public:
  explicit Timer(std::string name);
  ~Timer() = default;

  double elapsed();
  void start();

 private:
  std::string name_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

}

#endif //SCOT_SRC_SOLVER_TIMER_H_
