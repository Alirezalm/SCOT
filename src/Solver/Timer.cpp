//
// Created by alireza on 25/07/22.
//

#include "Timer.h"

namespace Scot {

Timer::Timer(std::string name) {
  name_ = name;
}
double Timer::elapsed() {
  std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start_;
  auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(duration);
  return  static_cast<double>(duration.count());
}

void Timer::start() {
  start_ = std::chrono::high_resolution_clock::now();
}

}