//
// Created by alireza on 20/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKGATHERLOCALLINEAROUTERAPPROXIMATIONS_H_
#define DISCOT_SRC_TASKS_TASKGATHERLOCALLINEAROUTERAPPROXIMATIONS_H_

#include "TaskBase.h"
namespace scot {

class TaskGatherLocalLinearOuterApproximations : public TaskBase {

 public:
  explicit TaskGatherLocalLinearOuterApproximations(EnvironmentPtr env);
  ~TaskGatherLocalLinearOuterApproximations() override = default;

  void initialize() override;
  void execute() override;

};

}

#endif //DISCOT_SRC_TASKS_TASKGATHERLOCALLINEAROUTERAPPROXIMATIONS_H_
