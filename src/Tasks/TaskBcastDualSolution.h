//
// Created by alireza on 21/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKBCASTDUALSOLUTION_H_
#define DISCOT_SRC_TASKS_TASKBCASTDUALSOLUTION_H_

#include "TaskBase.h"
namespace Scot {

class TaskBcastDualSolution : public TaskBase {
 public:
  explicit TaskBcastDualSolution(EnvironmentPtr env);
  ~TaskBcastDualSolution() override = default;

  void initialize() override;
  void execute() override;

};

}

#endif //DISCOT_SRC_TASKS_TASKBCASTDUALSOLUTION_H_
