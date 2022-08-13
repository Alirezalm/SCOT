//
// Created by alireza on 12/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKADDDUALSOLUTION_H_
#define DISCOT_SRC_TASKS_TASKADDDUALSOLUTION_H_
#include "../Solver/Environment.h"
#include "TaskBase.h"
namespace scot {

class TaskAddDualSolution : public TaskBase{

 public:
  explicit TaskAddDualSolution(EnvironmentPtr env);
  ~TaskAddDualSolution() override = default;

  void initialize() override;
  void execute() override;

 private:
  DualSolution dual_solution_;
};


}

#endif //DISCOT_SRC_TASKS_TASKADDDUALSOLUTION_H_
