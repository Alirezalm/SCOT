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

  ModelPtr model_;

  ResultsPtr results_;

  TaskQueuePtr task_queue_ptr_;

  MipSolverPtr mip_solver_;

  MessagePassingInterfacePtr mpi_;

  LoggerPtr logger_;

  ReportPtr report_;

  SettingsPtr settings_;

  TimerPtr timer_;

};
using EnvironmentPtr = std::shared_ptr<Environment>;

}
#endif //DISCOT_SRC_ENVIRONMENT_H_
