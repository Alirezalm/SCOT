//
// Created by alireza on 12/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKADDLINEAROUTERAPPROXIMATION_H_
#define DISCOT_SRC_TASKS_TASKADDLINEAROUTERAPPROXIMATION_H_

#include "../Solver/Environment.h"
#include "TaskBase.h"
namespace Scot {

class TaskAddLinearOuterApproximation : public TaskBase {

 public:
  explicit TaskAddLinearOuterApproximation(EnvironmentPtr env);
  ~TaskAddLinearOuterApproximation() override = default;

  void initialize() override;
  void execute() override;
};

}

#endif //DISCOT_SRC_TASKS_TASKADDLINEAROUTERAPPROXIMATION_H_
