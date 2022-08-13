//
// Created by alireza on 12/07/22.
//

#ifndef SCOT_SRC_TASKS_TASKSOLVESINGLETREEDUALPROBLEM_H_
#define SCOT_SRC_TASKS_TASKSOLVESINGLETREEDUALPROBLEM_H_
#include "TaskBase.h"
namespace scot {

class TaskSolveSingleTreeDualProblem: public TaskBase{
 public:
  explicit TaskSolveSingleTreeDualProblem(EnvironmentPtr env);
  ~TaskSolveSingleTreeDualProblem() override = default;

  void initialize() override;
  void execute() override;

};

}


#endif //SCOT_SRC_TASKS_TASKSOLVESINGLETREEDUALPROBLEM_H_
