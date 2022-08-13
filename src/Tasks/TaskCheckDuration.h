//
// Created by alireza on 25/07/22.
//

#ifndef SCOT_SRC_TASKS_TASKCHECKDURATION_H_
#define SCOT_SRC_TASKS_TASKCHECKDURATION_H_
#include "TaskBase.h"
namespace scot {

class TaskCheckDuration : public TaskBase {
 public:
  explicit TaskCheckDuration(EnvironmentPtr env);
  ~TaskCheckDuration() override = default;

  void initialize() override;
  void execute() override;
};

}

#endif //SCOT_SRC_TASKS_TASKCHECKDURATION_H_
