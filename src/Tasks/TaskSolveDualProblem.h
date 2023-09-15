//
// Created by alireza on 12/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKSOLVEDUALPROBLEM_H_
#define DISCOT_SRC_TASKS_TASKSOLVEDUALPROBLEM_H_
#include "../Solver/Environment.h"
#include "TaskBase.h"
namespace Scot {

class TaskSolveDualProblem : public TaskBase {
 public:
  explicit TaskSolveDualProblem(EnvironmentPtr env);
  ~TaskSolveDualProblem() override = default;

  void initialize() override;
  void execute() override;
};

}

#endif //DISCOT_SRC_TASKS_TASKSOLVEDUALPROBLEM_H_
