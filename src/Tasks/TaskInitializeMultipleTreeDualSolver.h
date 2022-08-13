//
// Created by alireza on 08/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKINITIALIZEDUALSOLVER_H_
#define DISCOT_SRC_TASKS_TASKINITIALIZEDUALSOLVER_H_
#include "TaskBase.h"
namespace scot {
class TaskInitializeMultipleTreeDualSolver : public TaskBase {

 public:
  explicit TaskInitializeMultipleTreeDualSolver(EnvironmentPtr env);
  ~TaskInitializeMultipleTreeDualSolver() override = default;

  void initialize() override;
  void execute() override;
};
}

#endif //DISCOT_SRC_TASKS_TASKINITIALIZEDUALSOLVER_H_
