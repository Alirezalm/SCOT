//
// Created by alireza on 01/07/22.
//

#ifndef SCOT_SRC_TASKS_TASKGATHERLOCALQUADRATICOUTERAPPROXIMATIONS_H_
#define SCOT_SRC_TASKS_TASKGATHERLOCALQUADRATICOUTERAPPROXIMATIONS_H_
#include "TaskBase.h"
namespace Scot {

class TaskGatherLocalQuadraticOuterApproximations: public TaskBase{
 public:
  explicit TaskGatherLocalQuadraticOuterApproximations(EnvironmentPtr env);
  ~TaskGatherLocalQuadraticOuterApproximations() override = default;

  void initialize() override;

  void execute() override;
};


}

#endif //SCOT_SRC_TASKS_TASKGATHERLOCALQUADRATICOUTERAPPROXIMATIONS_H_
