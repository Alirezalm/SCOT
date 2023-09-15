//
// Created by alireza on 11/07/22.
//

#ifndef SCOT_SRC_TASKS_TASKINITIALIZESINGLETREEDUALSOLVER_H_
#define SCOT_SRC_TASKS_TASKINITIALIZESINGLETREEDUALSOLVER_H_
#include "TaskBase.h"

namespace Scot {

class TaskInitializeSingleTreeDualSolver : public TaskBase {
 public:
  explicit TaskInitializeSingleTreeDualSolver(EnvironmentPtr env);
  ~TaskInitializeSingleTreeDualSolver() override = default;

  void initialize() override;
  void execute() override;
};

}

#endif //SCOT_SRC_TASKS_TASKINITIALIZESINGLETREEDUALSOLVER_H_
