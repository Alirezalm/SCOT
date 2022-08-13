//
// Created by alireza on 08/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKCREATEMULTIPLETREEDUALPROBLEM_H_
#define DISCOT_SRC_TASKS_TASKCREATEMULTIPLETREEDUALPROBLEM_H_

#include "TaskBase.h"

namespace scot {

class TaskCreateMultipleTreeDualProblem : public TaskBase {

 public:
  explicit TaskCreateMultipleTreeDualProblem(EnvironmentPtr env);
  ~TaskCreateMultipleTreeDualProblem() override = default;

  void initialize() override;
  void execute() override;
};

}

#endif //DISCOT_SRC_TASKS_TASKCREATEMULTIPLETREEDUALPROBLEM_H_
