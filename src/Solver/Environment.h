//
// Created by alireza on 25/05/22.
//

#ifndef DISCOT_SRC_ENVIRONMENT_H_
#define DISCOT_SRC_ENVIRONMENT_H_
#include "../Model/Model.h"
#include "Structs.h"
#include "settings.h"

namespace Scot {

class Environment {

 public:
  Environment() = default;
  ~Environment() = default;

  ModelPtr Model;

  ResultsPtr Results;

  TaskQueuePtr TaskQueue;

  MipSolverPtr MipSolver;

  MessagePassingInterfacePtr Mpi;

  LoggerPtr Logger;

  ReportPtr Report;

  SettingsPtr Settings;

  TimerPtr Timer;

};
using EnvironmentPtr = std::shared_ptr<Environment>;

}
#endif //DISCOT_SRC_ENVIRONMENT_H_
